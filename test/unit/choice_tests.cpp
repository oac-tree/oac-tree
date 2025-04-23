/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/variables/local_variable.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

static const sup::dto::char8 *testTable[][2] = {{"{\"type\":\"uint8\"}", "0"},
                                                  {"{\"type\":\"uint8\"}", "1"},
                                                  {"{\"type\":\"uint8\"}", "2"},
                                                  {"{\"type\":\"uint8\"}", "3"},
                                                  {NULL}};
static sup::dto::uint8 resVal[] = {1, 2, 3, 0};
static ExecutionStatus statuses[] = {
  ExecutionStatus::SUCCESS,
  ExecutionStatus::SUCCESS,
  ExecutionStatus::SUCCESS,
  ExecutionStatus::FAILURE,
};

TEST(Choice, Default)  // Static initialisation
{
  const std::string body{R"(
   <Choice varName="sel">
        <Copy name="zero" inputVar="a" outputVar="res" />
        <Copy name="one" inputVar="b" outputVar="res" />
        <Copy name="two" inputVar="c" outputVar="res" />
    </Choice>
    <Workspace>
        <Local name="res"
               type='{"type":"int8"}'
               value='0' />
        <Local name="a"
               type='{"type":"int8"}'
               value='1' />
        <Local name="b"
               type='{"type":"int8"}'
               value='2' />
        <Local name="c"
               type='{"type":"int8"}'
               value='3' />
    </Workspace>
)"};

  auto proc_str = sup::UnitTestHelper::CreateProcedureString(body);

  std::size_t i = 0u;
  while ((testTable[i][0] != NULL))
  {
    auto proc = ParseProcedureString(proc_str);

    std::unique_ptr<Variable> varX(new LocalVariable);

    varX->AddAttribute("type", testTable[i][0]);
    varX->AddAttribute("value", testTable[i][1]);
    proc->AddVariable("sel", std::move(varX));

    sup::UnitTestHelper::EmptyUserInterface ui;
    proc->Setup();
    while ((proc->GetStatus() != ExecutionStatus::SUCCESS)
           && (proc->GetStatus() != ExecutionStatus::FAILURE))
    {
      proc->ExecuteSingle(ui);
    }
    EXPECT_EQ(proc->GetStatus(), statuses[i]);

    sup::dto::AnyValue result;
    proc->GetVariableValue("res", result);
    auto checkVal = result.As<sup::dto::uint8>();
    EXPECT_EQ(checkVal, resVal[i]);

    i++;
  }
}

TEST(Choice, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Choice");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  auto wait = GlobalInstructionRegistry().Create("Wait");
  ASSERT_TRUE(AppendChildInstruction(*instr, std::move(wait)));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(Choice, ArraySuccess)
{
  const std::string body{R"(
    <Choice varName="choice">
        <Counter/>
        <Counter incr="2"/>
        <Wait/>
        <Inverter>
            <Counter/>
        </Inverter>
    </Choice>
    <Workspace>
        <Local name="choice" type='{"type":"uint32_arr","multiplicity":3,"element":{"type":"uint32"}}' value="[0,1,2]"/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 3);
}

TEST(Choice, ArrayFailure)
{
  const std::string body{R"(
    <Choice varName="choice">
        <Counter/>
        <Counter incr="2"/>
        <Wait/>
        <Inverter>
            <Counter/>
        </Inverter>
    </Choice>
    <Workspace>
        <Local name="choice" type='{"type":"uint32_arr","multiplicity":3,"element":{"type":"uint32"}}' value="[1,2,3]"/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  // Instruction called and return failure
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 3);
}

TEST(Choice, NoSuchVariable)
{
  const std::string body{R"(
    <Choice varName="undefined">"
        <Counter/>"
        <Wait/>"
    </Choice>"
    <Workspace/>"
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  // Expect failure during execution
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Choice, NoAttribute)
{
  const std::string body{R"(
    <Choice>"
        <Counter/>"
        <Wait/>"
    </Choice>"
    <Workspace/>"
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  // Expect failure in Setup
  EXPECT_THROW(sup::UnitTestHelper::TryAndExecute(proc, ui), InstructionSetupException);
}
