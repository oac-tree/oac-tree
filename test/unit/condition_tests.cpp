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

#include <sup/oac-tree/instructions/condition.h>

#include "unit_test_helper.h"

#include <sup/oac-tree/variables/local_variable.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

static const sup::dto::char8 *conditionTable[][14] = {
    {"c", "{\"type\":\"uint8\"}", "0", "c", "false", NULL},
    {"c", "{\"type\":\"uint8\"}", "255", "c", "true", NULL},
    {"c", "{\"type\":\"uint64\"}", "10", "c", "true", NULL},
    {"c.field1",
     "{\"type\":\"StructuredData1\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}",
     "{\"field1\":2}", "c", "true", NULL},
    {"c[1].field1",
     "{\"type\":\"StructuredData6a\", \"multiplicity\":2, "
     "\"element\":{\"type\":\"StructuredData6Base\", "
     "\"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}",
     "[{\"field1\":1}, {\"field1\":0}]", "c", "false", NULL},
    {NULL}};

TEST(Condition, Default)
{
  const std::string body{R"(
    <Sequence>
        <Condition name="Condition" varName="a" />
    </Sequence>
    <Workspace>
        <Local name="a"
               type='{"type":"int8"}'
               value='1' />
        <Local name="b"
               type='{"type":"uint8"}'
               value='0' />
        <Local name="c"
               type='{"type":"uint16"}'
               value='3' />
        <Local name="d"
               type='{"type":"uint32"}'
               value='0' />
    </Workspace>
)"};

  const std::string file_name = "workspaceCondition.xml";
  sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);

  ASSERT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  proc->ExecuteSingle(ui);
  EXPECT_EQ(proc->GetStatus(), ExecutionStatus::SUCCESS);
}

TEST(Condition, DifferentTypes)
{
  bool status(true);

  std::size_t i = 0u;
  while ((conditionTable[i][0] != NULL) && status)
  {
    std::unique_ptr<Procedure> proc(new Procedure);
    std::unique_ptr<Condition> myCondNode(new Condition);
    myCondNode->AddAttribute("varName", conditionTable[i][0]);

    sup::dto::AnyValue resVal;

    std::unique_ptr<Variable> varX(new LocalVariable);
    varX->AddAttribute("type", conditionTable[i][1]);
    varX->AddAttribute("value", conditionTable[i][2]);

    varX->GetValue(resVal);
    proc->AddVariable(conditionTable[i][3], std::move(varX));

    if (status)
    {
      sup::UnitTestHelper::EmptyUserInterface ui;
      proc->PushInstruction(std::move(myCondNode));
      proc->Setup();
      proc->ExecuteSingle(ui);

      ::std::string result = conditionTable[i][4];
      if (result == "true")
      {
        status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
      }
      else
      {
        status = (proc->GetStatus() == ExecutionStatus::FAILURE);
      }
    }

    i++;
  }

  EXPECT_TRUE(status);
}

TEST(Condition, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Condition");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(Condition, NonScalarVariable_success)
{
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" varName="struct.timestamp"/>
    </Repeat>
    <Workspace>
        <Local name="struct"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Condition, NonScalarVariable_failure)
{
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" varName="struct"/>
    </Repeat>
    <Workspace>
        <Local name="struct"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(Condition, NoSuchVariable_name)
{
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" varName="undefined"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(Condition, NoSuchVariable_attr)
{
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" varName="struct.array[0].node"/>
    </Repeat>
    <Workspace>
        <Local name="struct"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>"
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
