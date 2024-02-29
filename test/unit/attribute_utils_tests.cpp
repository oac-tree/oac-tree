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

#include <sup/sequencer/attribute_utils.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

const std::string kEmptyValue = "";
const std::string kStrAttrValue = "str_value";
const std::string kDoubleAttrValue = "3.14";
const std::string kBoolAttrValue = "true";

class AttributeUtilsTest : public ::testing::Test
{
protected:
  AttributeUtilsTest();
  virtual ~AttributeUtilsTest();
};

TEST_F(AttributeUtilsTest, ParseAttributeStringSupported)
{
  {
    // StringType
    auto result = utils::ParseAttributeString(sup::dto::StringType, kStrAttrValue);
    EXPECT_TRUE(result.first);
    EXPECT_FALSE(sup::dto::IsEmptyValue(result.second));
    ASSERT_EQ(result.second.GetType(), sup::dto::StringType);
    EXPECT_EQ(result.second.As<std::string>(), kStrAttrValue);
  }
  {
    // Float64Type
    auto result = utils::ParseAttributeString(sup::dto::Float64Type, kDoubleAttrValue);
    EXPECT_TRUE(result.first);
    EXPECT_FALSE(sup::dto::IsEmptyValue(result.second));
    ASSERT_EQ(result.second.GetType(), sup::dto::Float64Type);
    EXPECT_EQ(result.second.As<double>(), 3.14);
  }
  {
    // BooleanType: true
    auto result = utils::ParseAttributeString(sup::dto::BooleanType, kBoolAttrValue);
    EXPECT_TRUE(result.first);
    EXPECT_FALSE(sup::dto::IsEmptyValue(result.second));
    ASSERT_EQ(result.second.GetType(), sup::dto::BooleanType);
    EXPECT_EQ(result.second.As<bool>(), true);
  }
  {
    // BooleanType: false
    auto result = utils::ParseAttributeString(sup::dto::BooleanType, "non_recognized_gives_false");
    EXPECT_TRUE(result.first);
    EXPECT_FALSE(sup::dto::IsEmptyValue(result.second));
    ASSERT_EQ(result.second.GetType(), sup::dto::BooleanType);
    EXPECT_EQ(result.second.As<bool>(), false);
  }
}

TEST_F(AttributeUtilsTest, ParseAttributeStringFailed)
{
  {
    // Float64Type
    auto result = utils::ParseAttributeString(sup::dto::Float64Type, "cannot_be_parsed");
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
  {
    // Float64Type again...
    auto result = utils::ParseAttributeString(sup::dto::Float64Type, "1.2should_fail");
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
  {
    // UnsignedInteger32Type
    auto result = utils::ParseAttributeString(sup::dto::UnsignedInteger32Type, "-6");
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
}

TEST_F(AttributeUtilsTest, ParseAttributeStringNotSupported)
{
  {
    // EmptyType
    auto result = utils::ParseAttributeString(sup::dto::EmptyType, kEmptyValue);
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
  {
    // struct
    sup::dto::AnyType struct_type = {
      { "member", sup::dto::StringType }
    };
    auto result = utils::ParseAttributeString(struct_type, kEmptyValue);
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
  {
    // array
    sup::dto::AnyType array_type{5, sup::dto::StringType};
    auto result = utils::ParseAttributeString(array_type, kEmptyValue);
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
}

TEST_F(AttributeUtilsTest, DISABLED_ParseAttributeStringPermissive)
{
  {
    // SignedInteger32Type
    auto result = utils::ParseAttributeString(sup::dto::SignedInteger32Type, "3.14");
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(sup::dto::IsEmptyValue(result.second));
  }
}

TEST_F(AttributeUtilsTest, CreateAttributeStringSupported)
{
  {
    // StringType
    sup::dto::AnyValue val{sup::dto::StringType, kStrAttrValue};
    auto result = utils::CreateAttributeString(val);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, kStrAttrValue);
  }
  {
    // Float64Type
    sup::dto::AnyValue val{sup::dto::Float64Type, 3.14};
    auto result = utils::CreateAttributeString(val);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, kDoubleAttrValue);
  }
  {
    // BooleanType: true
    sup::dto::AnyValue val{sup::dto::BooleanType, true};
    auto result = utils::CreateAttributeString(val);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, "true");
  }
  {
    // BooleanType: false
    sup::dto::AnyValue val{sup::dto::BooleanType, false};
    auto result = utils::CreateAttributeString(val);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second, "false");
  }
}

TEST_F(AttributeUtilsTest, CreateAttributeStringNotSupported)
{
  {
    // EmptyType
    sup::dto::AnyValue val{};
    auto result = utils::CreateAttributeString(val);
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(result.second.empty());
  }
  {
    // struct
    sup::dto::AnyValue val = {
      { "member", { sup::dto::StringType, "member_value" }}
    };
    auto result = utils::CreateAttributeString(val);
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(result.second.empty());
  }
  {
    // array
    sup::dto::AnyValue val{5, sup::dto::StringType};
    auto result = utils::CreateAttributeString(val);
    EXPECT_FALSE(result.first);
    EXPECT_TRUE(result.second.empty());
  }
}

AttributeUtilsTest::AttributeUtilsTest() = default;

AttributeUtilsTest::~AttributeUtilsTest() = default;
