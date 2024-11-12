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
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include "unit_test_helper.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

class UserInterfaceTest : public ::testing::Test
{
protected:
  UserInterfaceTest();
  virtual ~UserInterfaceTest();

  MockUserInterface mock_ui;
  sup::UnitTestHelper::EmptyUserInterface empty_ui;
  Workspace empty_ws;
  std::unique_ptr<Instruction> wait;
};

const std::string TREE_TYPE = "TestData";

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

TEST_F(UserInterfaceTest, UpdateInstructionStatusDispatch)
{
  EXPECT_CALL(mock_ui,
              UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::NOT_STARTED)));
  mock_ui.UpdateInstructionStatus(wait.get());
}

TEST_F(UserInterfaceTest, PutValueDispatch)
{
  sup::dto::AnyValue val;
  std::string description = "TestPutValue";
  EXPECT_CALL(mock_ui, PutValue(_, description))
      .Times(2)
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_ui, GetUserValue(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserChoice(_, _)).Times(0);
  EXPECT_FALSE(mock_ui.PutValue(val, description));
  EXPECT_TRUE(mock_ui.PutValue(val, description));
}

TEST_F(UserInterfaceTest, GetUserValueDispatch)
{
  sup::dto::AnyValue val;
  std::string description = "TestGetUserValue";
  EXPECT_CALL(mock_ui, PutValue(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserValue(_, description))
      .Times(2)
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_ui, GetUserChoice(_, _)).Times(0);
  EXPECT_FALSE(mock_ui.GetUserValue(val, description));
  EXPECT_TRUE(mock_ui.GetUserValue(val, description));
}

TEST_F(UserInterfaceTest, GetUserChoiceDispatch)
{
  std::vector<std::string> options = {"yes", "no" };
  auto metadata = CreateUserChoiceMetadata();
  EXPECT_CALL(mock_ui, PutValue(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserValue(_, _)).Times(0);
  EXPECT_CALL(mock_ui, GetUserChoice(options, metadata))
      .Times(2)
      .WillOnce(Return(-1))
      .WillOnce(Return(0));
  EXPECT_EQ(mock_ui.GetUserChoice(options, metadata), -1);
  EXPECT_EQ(mock_ui.GetUserChoice(options, metadata), 0);
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
  auto user_value_request = CreateUserValueRequest(val, description);
  auto future = empty_ui.RequestUserInput(user_value_request);
  EXPECT_FALSE(future->IsValid());
  auto user_value_reply = GetBlockingUserValue(empty_ui, val, description);
  EXPECT_FALSE(user_value_reply.first);
  EXPECT_TRUE(sup::dto::IsEmptyValue(user_value_reply.second));
}

TEST_F(UserInterfaceTest, GetUserChoiceDefault)
{
  std::vector<std::string> options;
  auto user_choice_request = CreateUserChoiceRequest(options, {});
  auto future = empty_ui.RequestUserInput(user_choice_request);
  EXPECT_FALSE(future->IsValid());
  auto user_choice_reply = GetBlockingUserChoice(empty_ui, options, {});
  EXPECT_FALSE(user_choice_reply.first);
  EXPECT_EQ(user_choice_reply.second, -1);
}

TEST_F(UserInterfaceTest, InstructionExecution)
{
  EXPECT_THAT(wait, HasExecutionStatus(ExecutionStatus::NOT_STARTED));
  {
    InSequence seq;
    EXPECT_CALL(mock_ui,
                UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(HasExecutionStatus(ExecutionStatus::SUCCESS)));
  }
  wait->ExecuteSingle(mock_ui, empty_ws);
}

UserInterfaceTest::UserInterfaceTest()
    : mock_ui{}, empty_ui{}, empty_ws{}, wait{GlobalInstructionRegistry().Create("Wait")}
{}

UserInterfaceTest::~UserInterfaceTest() = default;
