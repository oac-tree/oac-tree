/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class InputTest : public ::testing::Test
{
protected:
  InputTest() = default;
  virtual ~InputTest() = default;
};

TEST_F(InputTest, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Input");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("outputVar", "out_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST_F(InputTest, GetUserValueSuccess)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" outputVar="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="uint32" type='{"type":"uint32"}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  sup::dto::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecuteNoReset(proc, ui,
    ExecutionStatus::SUCCESS));
  EXPECT_TRUE(sup::dto::UnsignedInteger32Type == ui.GetType());
  sup::dto::AnyValue var_value;
  EXPECT_TRUE(proc->GetVariableValue("uint32", var_value));
  EXPECT_EQ(var_value.As<sup::dto::uint32>(), 1234u);
}

TEST_F(InputTest, GetUserValueFailure)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" outputVar="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="uint32" type='{"type":"uint32"}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ui.SetStatus(false);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(InputTest, VariableUninitialised)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" outputVar="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="uint32"/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  sup::dto::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(InputTest, VariableUndefined)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" outputVar="uint32"/>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  sup::dto::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(InputTest, VariableDescription)
{
  const std::string body{R"(
    <Sequence>
        <Input description="@descr" outputVar="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="descr" type='{"type":"string"}' value='"Give me a number"'/>
        <Local name="uint32" type='{"type":"uint32"}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  sup::dto::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
