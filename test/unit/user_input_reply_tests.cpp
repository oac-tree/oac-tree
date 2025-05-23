/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
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

#include <sup/oac-tree/user_input_reply.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class UserInputReplyTest : public ::testing::Test
{
protected:
  UserInputReplyTest() = default;
  virtual ~UserInputReplyTest() = default;
};

TEST_F(UserInputReplyTest, CreateUserValueReply)
{
  {
    // User value reply that indicates success
    sup::dto::AnyValue user_value{ sup::dto::Character8Type, 'a' };
    auto user_reply = CreateUserValueReply(true, user_value);
    EXPECT_EQ(user_reply.m_request_type, InputRequestType::kUserValue);
    EXPECT_TRUE(user_reply.m_result);
    EXPECT_EQ(user_reply.m_payload, user_value);

    // Parse the AnyValue back
    auto [parsed, reply] = ParseUserValueReply(user_reply);
    EXPECT_TRUE(parsed);
    ASSERT_EQ(reply.GetType(), sup::dto::Character8Type);
    EXPECT_EQ(reply.As<sup::dto::char8>(), 'a');
  }
  {
    // User value reply that indicates failure
    sup::dto::AnyValue user_value{ sup::dto::Character8Type, 'a' };
    auto user_reply = CreateUserValueReply(false, user_value);
    EXPECT_EQ(user_reply.m_request_type, InputRequestType::kUserValue);
    EXPECT_FALSE(user_reply.m_result);
    EXPECT_EQ(user_reply.m_payload, user_value);

    // Parse the AnyValue back
    auto [parsed, reply] = ParseUserValueReply(user_reply);
    EXPECT_FALSE(parsed);
    // During parsing of a failure reply, the payload is always an empty AnyValue
    EXPECT_TRUE(sup::dto::IsEmptyValue(reply));
  }
}

TEST_F(UserInputReplyTest, CreateUserChoiceReply)
{
  {
    // User choice reply that indicates success
    int choice = 5;
    auto user_reply = CreateUserChoiceReply(true, choice);
    EXPECT_EQ(user_reply.m_request_type, InputRequestType::kUserChoice);
    EXPECT_TRUE(user_reply.m_result);
    ASSERT_EQ(user_reply.m_payload.GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(user_reply.m_payload.As<sup::dto::int32>(), 5);

    // Parse the AnyValue back
    auto [parsed, reply] = ParseUserChoiceReply(user_reply);
    EXPECT_TRUE(parsed);
    EXPECT_EQ(reply, 5);
  }
  {
    // User choice reply that indicates failure
    int choice = 5;
    auto user_reply = CreateUserChoiceReply(false, choice);
    EXPECT_EQ(user_reply.m_request_type, InputRequestType::kUserChoice);
    EXPECT_FALSE(user_reply.m_result);
    ASSERT_EQ(user_reply.m_payload.GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(user_reply.m_payload.As<sup::dto::int32>(), 5);

    // Parse the AnyValue back
    auto [parsed, reply] = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed);
    // During parsing of a failure reply, the payload is always -1
    EXPECT_EQ(reply, -1);
  }
}

TEST_F(UserInputReplyTest, ParseUserValueReply)
{
  {
    // Successful parsing
    sup::dto::AnyValue payload{ sup::dto::StringType, "Hello" };
    UserInputReply user_reply{ InputRequestType::kUserValue, true, payload};
    auto [parsed, reply] = ParseUserValueReply(user_reply);
    EXPECT_TRUE(parsed);
    EXPECT_EQ(reply, payload);
  }
  {
    // Parsing of user reply with wrong type
    sup::dto::AnyValue payload{ sup::dto::StringType, "Hello" };
    UserInputReply user_reply{ InputRequestType::kUserChoice, true, payload};
    auto [parsed, reply] = ParseUserValueReply(user_reply);
    EXPECT_FALSE(parsed);
    EXPECT_TRUE(sup::dto::IsEmptyValue(reply));
  }
  {
    // Parsing of failed user value input
    sup::dto::AnyValue payload{ sup::dto::StringType, "Hello" };
    UserInputReply user_reply{ InputRequestType::kUserValue, false, payload};
    auto [parsed, reply] = ParseUserValueReply(user_reply);
    EXPECT_FALSE(parsed);
    EXPECT_TRUE(sup::dto::IsEmptyValue(reply));
  }
}

TEST_F(UserInputReplyTest, ParseUserChoiceReply)
{
  {
    // Successful parsing
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserChoice, true, choice_av};
    auto [parsed, reply] = ParseUserChoiceReply(user_reply);
    EXPECT_TRUE(parsed);
    EXPECT_EQ(reply, choice);
  }
  {
    // Parsing of user choice with wrong type
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserValue, true, choice_av};
    auto [parsed, reply] = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed);
    EXPECT_EQ(reply, -1);
  }
  {
    // Parsing of failed user choice input
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserChoice, false, choice_av};
    auto [parsed, reply] = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed);
    EXPECT_EQ(reply, -1);
  }
  {
    // Parsing of user choice input with wrong type of payload
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::UnsignedInteger64Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserChoice, true, choice_av};
    auto [parsed, reply] = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed);
    EXPECT_EQ(reply, -1);
  }
}
