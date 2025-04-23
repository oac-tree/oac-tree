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

#include <sup/oac-tree/instructions/message.h>

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

//! Testing Message instruction.
class MessageTest : public ::testing::Test
{
public:
  //! Test interface for Message method.
  class TestInterface : public DefaultUserInterface
  {
  public:
    TestInterface() = default;

    void Message(const std::string& message) override { ui_message = message; }

    std::string ui_message;
  };

protected:
  Procedure proc;
  TestInterface ui;
  Workspace ws;
};

TEST_F(MessageTest, Setup)
{
  Message message_instr{};
  EXPECT_THROW(message_instr.Setup(proc), InstructionSetupException);

  message_instr.AddAttribute("name", "MyMessageInstruction");
  EXPECT_THROW(message_instr.Setup(proc), InstructionSetupException);

  message_instr.AddAttribute("text", "Hello world!");
  EXPECT_NO_THROW(message_instr.Setup(proc));
}

TEST_F(MessageTest, Execute)
{
  std::string message("Hello oac-tree!");
  Message message_instr{};
  message_instr.AddAttribute("text", message);
  EXPECT_NO_THROW(message_instr.Setup(proc));

  message_instr.ExecuteSingle(ui, ws);
  EXPECT_EQ(message_instr.GetStatus(), ExecutionStatus::SUCCESS);

  EXPECT_EQ(ui.ui_message, message);
}

TEST_F(MessageTest, FromRegistry)
{
  auto message_instr = GlobalInstructionRegistry().Create("Message");
  EXPECT_THROW(message_instr->Setup(proc), InstructionSetupException);

  std::string message = "Hello world!";
  message_instr->AddAttribute("text", message);
  EXPECT_NO_THROW(message_instr->Setup(proc));
  ui.ui_message = "UNDEFINED";

  message_instr->ExecuteSingle(ui, ws);
  EXPECT_EQ(message_instr->GetStatus(), ExecutionStatus::SUCCESS);

  EXPECT_EQ(ui.ui_message, message);
}

TEST_F(MessageTest, VariableText)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Message text="@msg"/>
  <Workspace>
    <Local name="msg" type='{"type":"string"}' value='"Hello"'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(MessageTest, VariableTextWrongType)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Message text="@msg"/>
  <Workspace>
    <Local name="msg" type='{"type":"int16"}' value='42'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(MessageTest, VariableTextNotPresent)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Message text="@msg"/>
  <Workspace>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

