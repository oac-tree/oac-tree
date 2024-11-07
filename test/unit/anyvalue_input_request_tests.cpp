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

#include <sup/sequencer/anyvalue_input_request.h>
#include <sup/sequencer/constants.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class AnyValueInputRequestTest : public ::testing::Test
{
protected:
  AnyValueInputRequestTest() = default;
  virtual ~AnyValueInputRequestTest() = default;
};

TEST_F(AnyValueInputRequestTest, CreateUserValueRequest)
{
  sup::dto::AnyType input_type = {{
    { "a", sup::dto::Float64Type },
    { "indices", sup::dto::AnyType{5, sup::dto::UnsignedInteger16Type }}
  }};
  sup::dto::AnyValue input{input_type};
  std::string description = "Set configuration";
  auto request = CreateUserValueRequest(input, description);

  // Check request fields
  EXPECT_EQ(request.m_request_type, InputRequestType::kUserValue);
  EXPECT_EQ(request.m_meta_data, sup::dto::AnyValue{description});
  EXPECT_EQ(request.m_input_type, input_type);

  // Parse this request
  sup::dto::AnyValue value_parsed{};
  std::string description_parsed{};
  EXPECT_TRUE(ParseUserValueRequest(request, value_parsed, description_parsed));
  EXPECT_EQ(value_parsed, input);
  EXPECT_EQ(description_parsed, description);
}

TEST_F(AnyValueInputRequestTest, CreateUserChoiceRequest)
{
  std::vector<std::string> options{ "start", "stop", "pause", "resume"};
  sup::dto::AnyValue metadata = {{
    { "a", "useful information about request"},
    { "flag", true }
  }};
  auto request = CreateUserChoiceRequest(options, metadata);

  // Check request fields
  EXPECT_EQ(request.m_request_type, InputRequestType::kUserChoice);
  EXPECT_EQ(request.m_input_type, sup::dto::SignedInteger32Type);
  ASSERT_TRUE(request.m_meta_data.HasField(Constants::kInputRequestOptionsFieldName));
  ASSERT_TRUE(request.m_meta_data.HasField(Constants::kInputRequestMetadataFieldName));
  EXPECT_EQ(request.m_meta_data[Constants::kInputRequestMetadataFieldName], metadata);
  auto& options_av = request.m_meta_data[Constants::kInputRequestOptionsFieldName];
  ASSERT_TRUE(sup::dto::IsArrayValue(options_av));
  ASSERT_EQ(options_av.GetType().ElementType(), sup::dto::StringType);
  ASSERT_EQ(options_av.NumberOfElements(), options.size());
  for (std::size_t idx = 0; idx < options.size(); ++idx)
  {
    EXPECT_EQ(options_av[idx].As<std::string>(), options[idx]);
  }

  // Parse this request
  std::vector<std::string> options_parsed;
  sup::dto::AnyValue meta_parsed;
  EXPECT_TRUE(ParseUserChoiceRequest(request, options_parsed, meta_parsed));
  EXPECT_EQ(options_parsed, options);
  EXPECT_EQ(meta_parsed, metadata);
}

TEST_F(AnyValueInputRequestTest, ParseUserValueRequest)
{
  {
    // Successful parsing
    const std::string description = "description";
    InputRequestType request_type = InputRequestType::kUserValue;
    sup::dto::AnyValue metadata{ sup::dto::StringType, description };
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    sup::dto::AnyValue parsed_value{};
    std::string parsed_description{};
    EXPECT_TRUE(ParseUserValueRequest(request, parsed_value, parsed_description));
    sup::dto::AnyValue expected_value{ sup::dto::StringType };
    EXPECT_EQ(parsed_value, expected_value);
    EXPECT_EQ(parsed_description, description);
  }
  {
    // Wrong request type
    const std::string description = "description";
    InputRequestType request_type = InputRequestType::kUserChoice;
    sup::dto::AnyValue metadata{ sup::dto::StringType, description };
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    sup::dto::AnyValue parsed_value{};
    std::string parsed_description{};
    EXPECT_FALSE(ParseUserValueRequest(request, parsed_value, parsed_description));
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed_value));
    EXPECT_TRUE(parsed_description.empty());
  }
  {
    // Wrong metadata type (must be StringType)
    InputRequestType request_type = InputRequestType::kUserValue;
    sup::dto::AnyValue metadata{ sup::dto::Float64Type, 3.14 };
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    sup::dto::AnyValue parsed_value{};
    std::string parsed_description{};
    EXPECT_FALSE(ParseUserValueRequest(request, parsed_value, parsed_description));
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed_value));
    EXPECT_TRUE(parsed_description.empty());
  }
  {
    // Pass AnyValue that can't be assigned to (array elements have a locked type)
    const std::string description = "description";
    InputRequestType request_type = InputRequestType::kUserValue;
    sup::dto::AnyValue metadata{ sup::dto::StringType, description };
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    // Array of 4 uint64 values; one of them will be used to simulate failed assignment
    sup::dto::AnyValue parsed_value{4, sup::dto::UnsignedInteger64Type};
    std::string parsed_description{};
    EXPECT_FALSE(ParseUserValueRequest(request, parsed_value[0], parsed_description));
    EXPECT_EQ(parsed_value.NumberOfElements(), 4);
    EXPECT_EQ(parsed_value.GetType().ElementType(), sup::dto::UnsignedInteger64Type);
    EXPECT_TRUE(parsed_description.empty());
  }
}

TEST_F(AnyValueInputRequestTest, ParseUserChoiceRequest)
{
  {
    // Successful parsing
    InputRequestType request_type = InputRequestType::kUserChoice;
    std::vector<std::string> options{ "one", "two", "three" };
    sup::dto::AnyValue options_av{options.size(), sup::dto::StringType};
    for (size_t idx = 0; idx < options.size(); ++idx)
    {
      options_av[idx] = options[idx];
    }
    const std::string metatext = "meta";
    sup::dto::AnyValue metadata{{
      { Constants::kInputRequestOptionsFieldName, options_av },
      { Constants::kInputRequestMetadataFieldName, { sup::dto::StringType, metatext }}
    }};
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    std::vector<std::string> parsed_options{};
    sup::dto::AnyValue parsed_metadata{};
    EXPECT_TRUE(ParseUserChoiceRequest(request, parsed_options, parsed_metadata));
    sup::dto::AnyValue expected_metadata{ sup::dto::StringType, metatext };
    EXPECT_EQ(parsed_metadata, expected_metadata);
    EXPECT_EQ(parsed_options, options);
  }
  {
    // Wrong request type
    InputRequestType request_type = InputRequestType::kUserValue;
    std::vector<std::string> options{ "one", "two", "three" };
    sup::dto::AnyValue options_av{options.size(), sup::dto::StringType};
    for (size_t idx = 0; idx < options.size(); ++idx)
    {
      options_av[idx] = options[idx];
    }
    const std::string metatext = "meta";
    sup::dto::AnyValue metadata{{
      { Constants::kInputRequestOptionsFieldName, options_av },
      { Constants::kInputRequestMetadataFieldName, { sup::dto::StringType, metatext }}
    }};
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    std::vector<std::string> parsed_options{};
    sup::dto::AnyValue parsed_metadata{};
    EXPECT_FALSE(ParseUserChoiceRequest(request, parsed_options, parsed_metadata));
    EXPECT_TRUE(parsed_options.empty());
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed_metadata));
  }
  {
    // Missing options field
    InputRequestType request_type = InputRequestType::kUserChoice;
    const std::string metatext = "meta";
    sup::dto::AnyValue metadata{{
      { Constants::kInputRequestMetadataFieldName, { sup::dto::StringType, metatext }}
    }};
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    std::vector<std::string> parsed_options{};
    sup::dto::AnyValue parsed_metadata{};
    EXPECT_FALSE(ParseUserChoiceRequest(request, parsed_options, parsed_metadata));
    EXPECT_TRUE(parsed_options.empty());
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed_metadata));
  }
  {
    // Options field not an array type
    InputRequestType request_type = InputRequestType::kUserChoice;
    sup::dto::AnyValue options_av{sup::dto::StringType};
    const std::string metatext = "meta";
    sup::dto::AnyValue metadata{{
      { Constants::kInputRequestOptionsFieldName, options_av },
      { Constants::kInputRequestMetadataFieldName, { sup::dto::StringType, metatext }}
    }};
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    std::vector<std::string> parsed_options{};
    sup::dto::AnyValue parsed_metadata{};
    EXPECT_FALSE(ParseUserChoiceRequest(request, parsed_options, parsed_metadata));
    EXPECT_TRUE(parsed_options.empty());
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed_metadata));
  }
  {
    // Options field array of wrong type
    InputRequestType request_type = InputRequestType::kUserChoice;
    sup::dto::AnyValue options_av{3, sup::dto::Float32Type};
    const std::string metatext = "meta";
    sup::dto::AnyValue metadata{{
      { Constants::kInputRequestOptionsFieldName, options_av },
      { Constants::kInputRequestMetadataFieldName, { sup::dto::StringType, metatext }}
    }};
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    std::vector<std::string> parsed_options{};
    sup::dto::AnyValue parsed_metadata{};
    EXPECT_FALSE(ParseUserChoiceRequest(request, parsed_options, parsed_metadata));
    EXPECT_TRUE(parsed_options.empty());
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed_metadata));
  }
  {
    // Pass AnyValue that can't be assigned to (array elements have a locked type)
    InputRequestType request_type = InputRequestType::kUserChoice;
    std::vector<std::string> options{ "one", "two", "three" };
    sup::dto::AnyValue options_av{options.size(), sup::dto::StringType};
    for (size_t idx = 0; idx < options.size(); ++idx)
    {
      options_av[idx] = options[idx];
    }
    const std::string metatext = "meta";
    sup::dto::AnyValue metadata{{
      { Constants::kInputRequestOptionsFieldName, options_av },
      { Constants::kInputRequestMetadataFieldName, { sup::dto::StringType, metatext }}
    }};
    sup::dto::AnyType input_type = sup::dto::StringType;
    AnyValueInputRequest request{ request_type, metadata, input_type };
    std::vector<std::string> parsed_options{};
    sup::dto::AnyValue parsed_metadata{4, sup::dto::UnsignedInteger64Type};
    EXPECT_FALSE(ParseUserChoiceRequest(request, parsed_options, parsed_metadata[0]));
    EXPECT_TRUE(parsed_options.empty());
    EXPECT_EQ(parsed_metadata.NumberOfElements(), 4);
    EXPECT_EQ(parsed_metadata.GetType().ElementType(), sup::dto::UnsignedInteger64Type);
  }
}

TEST_F(AnyValueInputRequestTest, CreateUserValueReply)
{
  {
    // User value reply that indicates success
    sup::dto::AnyValue user_value{ sup::dto::Character8Type, 'a' };
    auto reply_av = CreateUserValueReply(true, user_value);
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyResultFieldName));
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyValueFieldName));
    ASSERT_EQ(reply_av[Constants::kInputReplyResultFieldName].GetType(), sup::dto::BooleanType);
    EXPECT_EQ(reply_av[Constants::kInputReplyResultFieldName].As<sup::dto::boolean>(), true);
    ASSERT_EQ(reply_av[Constants::kInputReplyValueFieldName].GetType(), sup::dto::Character8Type);
    EXPECT_EQ(reply_av[Constants::kInputReplyValueFieldName].As<sup::dto::char8>(), 'a');

    // Parse the AnyValue back
    auto result = ParseUserValueReply(reply_av);
    EXPECT_EQ(result.first, true);
    ASSERT_EQ(result.second.GetType(), sup::dto::Character8Type);
    EXPECT_EQ(result.second.As<sup::dto::char8>(), 'a');
  }
  {
    // User value reply that indicates failure
    sup::dto::AnyValue user_value{ sup::dto::Character8Type, 'a' };
    auto reply_av = CreateUserValueReply(false, user_value);
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyResultFieldName));
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyValueFieldName));
    ASSERT_EQ(reply_av[Constants::kInputReplyResultFieldName].GetType(), sup::dto::BooleanType);
    EXPECT_EQ(reply_av[Constants::kInputReplyResultFieldName].As<sup::dto::boolean>(), false);
    ASSERT_EQ(reply_av[Constants::kInputReplyValueFieldName].GetType(), sup::dto::Character8Type);
    EXPECT_EQ(reply_av[Constants::kInputReplyValueFieldName].As<sup::dto::char8>(), 'a');

    // Parse the AnyValue back
    auto result = ParseUserValueReply(reply_av);
    EXPECT_EQ(result.first, false);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
}

TEST_F(AnyValueInputRequestTest, CreateUserChoiceReply)
{
  {
    // User choice reply that indicates success
    int choice = 5;
    auto reply_av = CreateUserChoiceReply(true, choice);
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyResultFieldName));
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyValueFieldName));
    ASSERT_EQ(reply_av[Constants::kInputReplyResultFieldName].GetType(), sup::dto::BooleanType);
    EXPECT_EQ(reply_av[Constants::kInputReplyResultFieldName].As<sup::dto::boolean>(), true);
    ASSERT_EQ(reply_av[Constants::kInputReplyValueFieldName].GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(reply_av[Constants::kInputReplyValueFieldName].As<sup::dto::int32>(), 5);

    // Parse the AnyValue back
    auto result = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(result.first, true);
    EXPECT_EQ(result.second, 5);
  }
  {
    // User choice reply that indicates failure
    int choice = 5;
    auto reply_av = CreateUserChoiceReply(false, choice);
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyResultFieldName));
    ASSERT_TRUE(reply_av.HasField(Constants::kInputReplyValueFieldName));
    ASSERT_EQ(reply_av[Constants::kInputReplyResultFieldName].GetType(), sup::dto::BooleanType);
    EXPECT_EQ(reply_av[Constants::kInputReplyResultFieldName].As<sup::dto::boolean>(), false);
    ASSERT_EQ(reply_av[Constants::kInputReplyValueFieldName].GetType(), sup::dto::SignedInteger32Type);
    EXPECT_EQ(reply_av[Constants::kInputReplyValueFieldName].As<sup::dto::int32>(), 5);

    // Parse the AnyValue back
    auto result = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(result.first, false);
    EXPECT_EQ(result.second, -1);
  }
}

TEST_F(AnyValueInputRequestTest, ParseUserValueReply)
{
  {
    // Successful parsing
    const std::string text = "some user provided value";
    sup::dto::AnyValue payload{ sup::dto::StringType, text };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, true },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserValueReply(reply_av);
    EXPECT_EQ(parsed.first, true);
    EXPECT_EQ(parsed.second, payload);
  }
  {
    // Parsing of user reply that failed
    const std::string text = "some user provided value";
    sup::dto::AnyValue payload{ sup::dto::StringType, text };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, false },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserValueReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed.second));
  }
  {
    // Missing result field
    const std::string text = "some user provided value";
    sup::dto::AnyValue payload{ sup::dto::StringType, text };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserValueReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed.second));
  }
  {
    // Wrong type of result field
    const std::string text = "some user provided value";
    sup::dto::AnyValue payload{ sup::dto::StringType, text };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, { sup::dto::StringType, "wrong_type" } },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserValueReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed.second));
  }
  {
    // Missing reply value field
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, true }
    }};
    auto parsed = ParseUserValueReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_TRUE(sup::dto::IsEmptyValue(parsed.second));
  }
}

TEST_F(AnyValueInputRequestTest, ParseUserChoiceReply)
{
  {
    // Successful parsing
    sup::dto::AnyValue payload{ sup::dto::SignedInteger32Type, 7 };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, true },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(parsed.first, true);
    EXPECT_EQ(parsed.second, 7);
  }
  {
    // Parsing of user reply that failed
    sup::dto::AnyValue payload{ sup::dto::SignedInteger32Type, 7 };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, false },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_EQ(parsed.second, -1);
  }
  {
    // Missing result field
    sup::dto::AnyValue payload{ sup::dto::SignedInteger32Type, 7 };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_EQ(parsed.second, -1);
  }
  {
    // Wrong type of result field
    sup::dto::AnyValue payload{ sup::dto::SignedInteger32Type, 7 };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, { sup::dto::StringType, "wrong_type" } },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_EQ(parsed.second, -1);
  }
  {
    // Missing user value field
    sup::dto::AnyValue payload{ sup::dto::SignedInteger32Type, 7 };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, true }
    }};
    auto parsed = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_EQ(parsed.second, -1);
  }
  {
    // Wrong type of user value field
    sup::dto::AnyValue payload{ sup::dto::UnsignedInteger16Type, 7u };
    sup::dto::AnyValue reply_av{{
      { Constants::kInputReplyResultFieldName, true },
      { Constants::kInputReplyValueFieldName, payload }
    }};
    auto parsed = ParseUserChoiceReply(reply_av);
    EXPECT_EQ(parsed.first, false);
    EXPECT_EQ(parsed.second, -1);
  }
}
