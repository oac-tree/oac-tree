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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mock_user_interface.h"
#include "unit_test_helper.h"

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/job_interface_adapter.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>
#include <sup/oac-tree/constants.h>

#include <gtest/gtest.h>
#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

using namespace sup::oac_tree;

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
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgReferee;

TEST_F(UserInterfaceTest, UpdateInstructionStatusDispatch)
{
  EXPECT_CALL(mock_ui, UpdateInstructionStatus(_, ExecutionStatus::NOT_STARTED));
  mock_ui.UpdateInstructionStatus(wait.get(), ExecutionStatus::NOT_STARTED);
}

TEST_F(UserInterfaceTest, PutValueDispatch)
{
  sup::dto::AnyValue val;
  std::string description = "TestPutValue";
  EXPECT_CALL(mock_ui, PutValue(_, description))
      .Times(2)
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_FALSE(mock_ui.PutValue(val, description));
  EXPECT_TRUE(mock_ui.PutValue(val, description));
}

TEST_F(UserInterfaceTest, UserChoiceMetadata)
{
  // Valid Metadata
  {
    auto valid_metadata = CreateUserChoiceMetadata();
    valid_metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, sup::dto::AnyValue{"Sample Text"});
    valid_metadata.AddMember(
        Constants::USER_CHOICES_DIALOG_TYPE_NAME,
        sup::dto::AnyValue{sup::dto::AnyType(sup::dto::UnsignedInteger32Type), 1});
    valid_metadata.AddMember(Constants::USER_CHOICES_MODAL_NAME, sup::dto::AnyValue{true});
    valid_metadata.AddMember(Constants::USER_CHOICES_TITLE_NAME,
                             sup::dto::AnyValue{"Sample Title"});
    valid_metadata.AddMember(Constants::USER_CHOICES_INFORMATIVE_TEXT_NAME,
                             sup::dto::AnyValue{"Informative Text"});
    valid_metadata.AddMember(Constants::USER_CHOICES_DETAILS_NAME, sup::dto::AnyValue{"Details"});

    EXPECT_TRUE(IsUserChoiceMetadata(valid_metadata));
    EXPECT_EQ(GetMainTextFromMetadata(valid_metadata), "Sample Text");
  }

  // Non-Struct Metadata
  {
    sup::dto::AnyValue non_struct_metadata{"Not a struct"};
    EXPECT_FALSE(IsUserChoiceMetadata(non_struct_metadata));
  }

  // Invalid Field Types
  {
    auto invalid_metadata = CreateUserChoiceMetadata();
    {
      invalid_metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, sup::dto::AnyValue{123});
      EXPECT_FALSE(IsUserChoiceMetadata(invalid_metadata));
    }
    {
      invalid_metadata.AddMember(Constants::USER_CHOICES_DIALOG_TYPE_NAME,
                                 sup::dto::AnyValue{"Invalid Type"});
      EXPECT_FALSE(IsUserChoiceMetadata(invalid_metadata));
    }
    {
      invalid_metadata.AddMember(Constants::USER_CHOICES_MODAL_NAME, sup::dto::AnyValue{"true"});
      EXPECT_FALSE(IsUserChoiceMetadata(invalid_metadata));
    }
    {
      invalid_metadata.AddMember(Constants::USER_CHOICES_TITLE_NAME, sup::dto::AnyValue{false});
      EXPECT_FALSE(IsUserChoiceMetadata(invalid_metadata));
    }
    {
      invalid_metadata.AddMember(Constants::USER_CHOICES_INFORMATIVE_TEXT_NAME,
                                 sup::dto::AnyValue{42});
      EXPECT_FALSE(IsUserChoiceMetadata(invalid_metadata));
    }
    {
      invalid_metadata.AddMember(Constants::USER_CHOICES_DETAILS_NAME,
                                 sup::dto::AnyValue{sup::dto::EmptyType});
      EXPECT_FALSE(IsUserChoiceMetadata(invalid_metadata));
    }
  }
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
  auto [retrieved, value] = GetBlockingUserValue(empty_ui, val, description);
  EXPECT_FALSE(retrieved);
  EXPECT_TRUE(sup::dto::IsEmptyValue(value));
}

TEST_F(UserInterfaceTest, CreateUserValueFutureDefaultScalar)
{
  sup::UnitTestHelper::MockJobInfoIO mock_job_io;
  Procedure proc{};
  JobInterfaceAdapter ui{proc, mock_job_io};
  auto instr = GlobalInstructionRegistry().Create("Wait");

  // Scalar input with default value
  sup::dto::AnyValue input{ sup::dto::UnsignedInteger16Type, 0 };
  std::string description = "Pick a number";
  sup::dto::AnyValue output{ sup::dto::UnsignedInteger16Type, 88 };
  EXPECT_CALL(mock_job_io, GetUserValue(_, input, description))
      .WillOnce(DoAll(SetArgReferee<1>(output), Return(true)));
  auto future = CreateUserValueFuture(ui, *instr, input, description);
  EXPECT_TRUE(future->WaitFor(1.0));
  auto reply = future->GetValue();
  EXPECT_EQ(reply.m_request_type, InputRequestType::kUserValue);
  EXPECT_TRUE(reply.m_result);
  EXPECT_EQ(reply.m_payload, output);
}

TEST_F(UserInterfaceTest, CreateUserValueFutureNonDefaultScalar)
{
  sup::UnitTestHelper::MockJobInfoIO mock_job_io;
  Procedure proc{};
  JobInterfaceAdapter ui{proc, mock_job_io};
  auto instr = GlobalInstructionRegistry().Create("Wait");

  // Scalar input with default value
  sup::dto::AnyValue input{ sup::dto::UnsignedInteger16Type, 42 };
  std::string description = "Pick a number";
  sup::dto::AnyValue output{ sup::dto::UnsignedInteger16Type, 88 };
  EXPECT_CALL(mock_job_io, GetUserValue(_, input, description))
      .WillOnce(DoAll(SetArgReferee<1>(output), Return(true)));
  auto future = CreateUserValueFuture(ui, *instr, input, description);
  EXPECT_TRUE(future->WaitFor(1.0));
  auto reply = future->GetValue();
  EXPECT_EQ(reply.m_request_type, InputRequestType::kUserValue);
  EXPECT_TRUE(reply.m_result);
  EXPECT_EQ(reply.m_payload, output);
}

TEST_F(UserInterfaceTest, CreateUserValueFutureDefaultStruct)
{
  sup::UnitTestHelper::MockJobInfoIO mock_job_io;
  Procedure proc{};
  JobInterfaceAdapter ui{proc, mock_job_io};
  auto instr = GlobalInstructionRegistry().Create("Wait");

  // Scalar input with default value
  sup::dto::AnyValue input{{
    { "flag", { sup::dto::BooleanType, false }},
    { "value", { sup::dto::Float32Type, 0.0f }}
  }};
  std::string description = "Set structure";
  sup::dto::AnyValue output{{
    { "flag", { sup::dto::BooleanType, true }},
    { "value", { sup::dto::Float32Type, 3.14f }}
  }};
  EXPECT_CALL(mock_job_io, GetUserValue(_, input, description))
      .WillOnce(DoAll(SetArgReferee<1>(output), Return(true)));
  auto future = CreateUserValueFuture(ui, *instr, input, description);
  EXPECT_TRUE(future->WaitFor(1.0));
  auto reply = future->GetValue();
  EXPECT_EQ(reply.m_request_type, InputRequestType::kUserValue);
  EXPECT_TRUE(reply.m_result);
  EXPECT_EQ(reply.m_payload, output);
}

TEST_F(UserInterfaceTest, CreateUserValueFutureNonDefaultStruct)
{
  sup::UnitTestHelper::MockJobInfoIO mock_job_io;
  Procedure proc{};
  JobInterfaceAdapter ui{proc, mock_job_io};
  auto instr = GlobalInstructionRegistry().Create("Wait");

  // Scalar input with default value
  sup::dto::AnyValue input{{
    { "flag", { sup::dto::BooleanType, true }},
    { "value", { sup::dto::Float32Type, 12.3f }}
  }};
  std::string description = "Set structure";
  sup::dto::AnyValue output{{
    { "flag", { sup::dto::BooleanType, false }},
    { "value", { sup::dto::Float32Type, 3.14f }}
  }};
  EXPECT_CALL(mock_job_io, GetUserValue(_, input, description))
      .WillOnce(DoAll(SetArgReferee<1>(output), Return(true)));
  auto future = CreateUserValueFuture(ui, *instr, input, description);
  EXPECT_TRUE(future->WaitFor(1.0));
  auto reply = future->GetValue();
  EXPECT_EQ(reply.m_request_type, InputRequestType::kUserValue);
  EXPECT_TRUE(reply.m_result);
  EXPECT_EQ(reply.m_payload, output);
}

TEST_F(UserInterfaceTest, GetUserChoiceDefault)
{
  std::vector<std::string> options;
  auto user_choice_request = CreateUserChoiceRequest(options, {});
  auto future = empty_ui.RequestUserInput(user_choice_request);
  EXPECT_FALSE(future->IsValid());
  auto [retrieved, value] = GetBlockingUserChoice(empty_ui, options, {});
  EXPECT_FALSE(retrieved);
  EXPECT_EQ(value, -1);
}

TEST_F(UserInterfaceTest, InstructionExecution)
{
  EXPECT_EQ(wait->GetStatus(), ExecutionStatus::NOT_STARTED);
  {
    InSequence seq;
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(_, ExecutionStatus::NOT_FINISHED));
    EXPECT_CALL(mock_ui, UpdateInstructionStatus(_, ExecutionStatus::SUCCESS));
  }
  wait->ExecuteSingle(mock_ui, empty_ws);
}

UserInterfaceTest::UserInterfaceTest()
    : mock_ui{}, empty_ui{}, empty_ws{}, wait{GlobalInstructionRegistry().Create("Wait")}
{}

UserInterfaceTest::~UserInterfaceTest() = default;
