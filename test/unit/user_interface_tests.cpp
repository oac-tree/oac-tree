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

#include "mock_user_interface.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

class EmptyUserInterface : public UserInterface
{
private:
  void UpdateInstructionStatusImpl(const Instruction* instruction) override{};
};

class UserInterfaceTest : public ::testing::Test
{
protected:
  UserInterfaceTest();
  virtual ~UserInterfaceTest();

  MockUserInterface mock_ui;
  EmptyUserInterface empty_ui;
  Workspace empty_ws;
  std::unique_ptr<Instruction> wait;
};

static const std::string TREE_TYPE = "TestData";

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

TEST_F(UserInterfaceTest, UpdateInstructionStatusDispatch)
{
  EXPECT_CALL(mock_ui,
              UpdateInstructionStatusImpl(HasExecutionStatus(ExecutionStatus::NOT_STARTED)));
  mock_ui.UpdateInstructionStatus(wait.get());
}

TEST_F(UserInterfaceTest, PutValueDispatch)
{
  sup::dto::AnyValue val;
  std::string description = "TestPutValue";
  EXPECT_CALL(mock_ui, PutValueImpl(_, description))
      .Times(2)
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_ui, GetUserValueImpl(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserChoiceImpl(_, _)).Times(0);
  EXPECT_FALSE(mock_ui.PutValue(val, description));
  EXPECT_TRUE(mock_ui.PutValue(val, description));
}

TEST_F(UserInterfaceTest, GetUserValueDispatch)
{
  sup::dto::AnyValue val;
  std::string description = "TestGetUserValue";
  EXPECT_CALL(mock_ui, PutValueImpl(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserValueImpl(_, description))
      .Times(2)
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_ui, GetUserChoiceImpl(_, _)).Times(0);
  EXPECT_FALSE(mock_ui.GetUserValue(val, description));
  EXPECT_TRUE(mock_ui.GetUserValue(val, description));
}

TEST_F(UserInterfaceTest, GetUserChoiceDispatch)
{
  std::vector<std::string> choices = {"yes", "no"};
  std::string description = "TestGetUserChoice";
  EXPECT_CALL(mock_ui, PutValueImpl(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserValueImpl(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserChoiceImpl(choices, description))
      .Times(2)
      .WillOnce(Return(-1))
      .WillOnce(Return(0));
  EXPECT_EQ(mock_ui.GetUserChoice(choices, description), -1);
  EXPECT_EQ(mock_ui.GetUserChoice(choices, description), 0);
}

TEST_F(UserInterfaceTest, StartSingleStepDispatch)
{
  EXPECT_CALL(mock_ui, StartSingleStepImpl()).Times(3);
  EXPECT_NO_THROW(mock_ui.StartSingleStep());
  EXPECT_NO_THROW(mock_ui.StartSingleStep());
  EXPECT_NO_THROW(mock_ui.StartSingleStep());
}

TEST_F(UserInterfaceTest, EndSingleStepDispatch)
{
  EXPECT_CALL(mock_ui, EndSingleStepImpl()).Times(5);
  EXPECT_NO_THROW(mock_ui.EndSingleStep());
  EXPECT_NO_THROW(mock_ui.EndSingleStep());
  EXPECT_NO_THROW(mock_ui.EndSingleStep());
  EXPECT_NO_THROW(mock_ui.EndSingleStep());
  EXPECT_NO_THROW(mock_ui.EndSingleStep());
}

TEST_F(UserInterfaceTest, PutValueDefault)
{
  sup::dto::AnyValue val;
  std::string description;
  EXPECT_FALSE(empty_ui.PutValue(val, description));
}

TEST_F(UserInterfaceTest, GetUserValueDefault)
{
  sup::dto::AnyValue val;
  std::string description;
  EXPECT_FALSE(empty_ui.GetUserValue(val, description));
}

TEST_F(UserInterfaceTest, GetUserChoiceDefault)
{
  std::vector<std::string> choices;
  std::string description;
  EXPECT_EQ(empty_ui.GetUserChoice(choices, description), -1);
}

TEST_F(UserInterfaceTest, StartSingleStepDefault)
{
  EXPECT_NO_THROW(empty_ui.StartSingleStep());
}

TEST_F(UserInterfaceTest, EndSingleStepDefault)
{
  EXPECT_NO_THROW(empty_ui.EndSingleStep());
}

TEST_F(UserInterfaceTest, InstructionExecution)
{
  EXPECT_THAT(wait, HasExecutionStatus(ExecutionStatus::NOT_STARTED));
  {
    InSequence seq;
    EXPECT_CALL(mock_ui,
                UpdateInstructionStatusImpl(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatusImpl(HasExecutionStatus(ExecutionStatus::SUCCESS)));
  }
  wait->ExecuteSingle(mock_ui, empty_ws);
}

UserInterfaceTest::UserInterfaceTest()
    : mock_ui{}, empty_ui{}, empty_ws{}, wait{GlobalInstructionRegistry().Create("Wait")}
{
}

UserInterfaceTest::~UserInterfaceTest() = default;
