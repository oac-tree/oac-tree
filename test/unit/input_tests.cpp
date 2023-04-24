/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

TEST(Input, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Input");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("output", "out_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(Input, GetUserValue_success)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
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

TEST(Input, GetUserValue_failure)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
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

TEST(Input, Variable_uninitialised)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
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

TEST(Input, Variable_undefined)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
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

// ToDo - Workspace variable not updated
