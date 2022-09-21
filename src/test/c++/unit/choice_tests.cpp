/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "log_ui.h"
#include "unit_test_helper.h"

#include <sup/sequencer/variables/local_variable.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/log.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

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
   <Choice var_name="sel">
        <Copy name="zero" input="a" output="res" />
        <Copy name="one" input="b" output="res" />
        <Copy name="two" input="c" output="res" />
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
    auto proc = sup::sequencer::ParseProcedureString(proc_str);

    std::unique_ptr<Variable> varX(new LocalVariable);

    varX->AddAttribute("type", testTable[i][0]);
    varX->AddAttribute("value", testTable[i][1]);
    proc->AddVariable("sel", varX.release());

    LogUI ui;
    proc->Setup();
    while ((proc->GetStatus() != ExecutionStatus::SUCCESS)
           && (proc->GetStatus() != ExecutionStatus::FAILURE))
    {
      proc->ExecuteSingle(&ui);
    }
    EXPECT_EQ(proc->GetStatus(), statuses[i]);

    sup::dto::AnyValue result;
    proc->GetVariableValue("res", result);
    auto checkVal = result.As<sup::dto::uint8>();
    EXPECT_EQ(checkVal, resVal[i]);

    i++;
  }
}

TEST(Choice, ArraySuccess)
{
  const std::string body{R"(
    <Choice var_name="choice">
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
      sup::sequencer::ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  ASSERT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 3);
}

TEST(Choice, ArrayFailure)
{
  const std::string body{R"(
    <Choice var_name="choice">
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
      sup::sequencer::ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  // Instruction called and return failure
  ASSERT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
  ASSERT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 3);
}

TEST(Choice, NoSuchVariable)
{
  const std::string body{R"(
    <Choice var_name="undefined">"
        <Counter/>"
        <Wait/>"
    </Choice>"
    <Workspace/>"
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  // Expect failure during execution
  ASSERT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
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
      sup::sequencer::ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  // Expect failure in Setup
  ASSERT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}
