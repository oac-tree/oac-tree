/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
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

#include <sup/sequencer/user_input_reply.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

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
    auto result = ParseUserValueReply(user_reply);
    EXPECT_EQ(result.first, true);
    ASSERT_EQ(result.second.GetType(), sup::dto::Character8Type);
    EXPECT_EQ(result.second.As<sup::dto::char8>(), 'a');
  }
  {
    // User value reply that indicates failure
    sup::dto::AnyValue user_value{ sup::dto::Character8Type, 'a' };
    auto user_reply = CreateUserValueReply(false, user_value);
    EXPECT_EQ(user_reply.m_request_type, InputRequestType::kUserValue);
    EXPECT_FALSE(user_reply.m_result);
    EXPECT_EQ(user_reply.m_payload, user_value);

    // Parse the AnyValue back
    auto result = ParseUserValueReply(user_reply);
    EXPECT_EQ(result.first, false);
    // During parsing of a failure reply, the payload is always an empty AnyValue
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
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
    auto result = ParseUserChoiceReply(user_reply);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, 5);
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
    auto result = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(result.first);
    // During parsing of a failure reply, the payload is always -1
    EXPECT_EQ(result.second, -1);
  }
}

TEST_F(UserInputReplyTest, ParseUserValueReply)
{
  {
    // Successful parsing
    sup::dto::AnyValue payload{ sup::dto::StringType, "Hello" };
    UserInputReply user_reply{ InputRequestType::kUserValue, true, payload};
    auto parsed = ParseUserValueReply(user_reply);
    EXPECT_TRUE(parsed.first);
    EXPECT_EQ(parsed.second, payload);
  }
  {
    // Parsing of user reply with wrong type
    sup::dto::AnyValue payload{ sup::dto::StringType, "Hello" };
    UserInputReply user_reply{ InputRequestType::kUserChoice, true, payload};
    auto parsed = ParseUserValueReply(user_reply);
    EXPECT_FALSE(parsed.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed.second));
  }
  {
    // Parsing of failed user value input
    sup::dto::AnyValue payload{ sup::dto::StringType, "Hello" };
    UserInputReply user_reply{ InputRequestType::kUserValue, false, payload};
    auto parsed = ParseUserValueReply(user_reply);
    EXPECT_FALSE(parsed.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed.second));
  }
}

TEST_F(UserInputReplyTest, ParseUserChoiceReply)
{
  {
    // Successful parsing
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserChoice, true, choice_av};
    auto parsed = ParseUserChoiceReply(user_reply);
    EXPECT_TRUE(parsed.first);
    EXPECT_EQ(parsed.second, choice);
  }
  {
    // Parsing of user choice with wrong type
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserValue, true, choice_av};
    auto parsed = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed.first);
    EXPECT_EQ(parsed.second, -1);
  }
  {
    // Parsing of failed user choice input
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::SignedInteger32Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserChoice, false, choice_av};
    auto parsed = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed.first);
    EXPECT_EQ(parsed.second, -1);
  }
  {
    // Parsing of user choice input with wrong type of payload
    int choice = 42;
    sup::dto::AnyValue choice_av{ sup::dto::UnsignedInteger64Type, choice };
    UserInputReply user_reply{ InputRequestType::kUserChoice, true, choice_av};
    auto parsed = ParseUserChoiceReply(user_reply);
    EXPECT_FALSE(parsed.first);
    EXPECT_EQ(parsed.second, -1);
  }
}
