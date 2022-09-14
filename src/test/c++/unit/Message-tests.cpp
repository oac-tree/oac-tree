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

#include <sup/sequencer/instructions/message.h>

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

//! Testing Message instruction.
class MessageTest : public ::testing::Test
{
public:
  //! Test interface for Message method.
  class TestInterface : public ::sup::sequencer::UserInterface
  {
  public:
    TestInterface() = default;

    void UpdateInstructionStatusImpl(const sup::sequencer::Instruction *instruction) override {}
    void MessageImpl(const std::string& message) override { ui_message = message; }

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
  EXPECT_TRUE(message_instr.Setup(proc));

  message_instr.AddAttribute("name", "MyMessageInstruction");
  EXPECT_TRUE(message_instr.Setup(proc));

  message_instr.AddAttribute("text", "Hello world!");
  EXPECT_TRUE(message_instr.Setup(proc));
}

TEST_F(MessageTest, Execute)
{
  std::string message("Hello Sequencer!");
  Message message_instr{};
  message_instr.AddAttribute("text", message);
  EXPECT_TRUE(message_instr.Setup(proc));

  message_instr.ExecuteSingle(&ui, &ws);
  EXPECT_EQ(message_instr.GetStatus(), ExecutionStatus::SUCCESS);

  EXPECT_EQ(ui.ui_message, message);
}

TEST_F(MessageTest, FromRegistry)
{
  auto message_instr = GlobalInstructionRegistry().Create("Message");
  EXPECT_TRUE(message_instr->Setup(proc));
  ui.ui_message = "UNDEFINED";

  message_instr->ExecuteSingle(&ui, &ws);
  EXPECT_EQ(message_instr->GetStatus(), ExecutionStatus::SUCCESS);

  EXPECT_EQ(ui.ui_message, "");
}
