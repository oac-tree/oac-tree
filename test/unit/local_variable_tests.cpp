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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/variables/local_variable.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <future>
#include <sstream>

using namespace sup::oac_tree;

class LocalVariableTest : public ::testing::Test
{
protected:
  LocalVariableTest();
  virtual ~LocalVariableTest();

  void SetupVariables();

  Workspace ws;
  LocalVariable empty_var;
  LocalVariable bool_var;
  LocalVariable uint64_var;
  LocalVariable float32_var;
  StringAttributeList attr_partial;
  StringAttributeList attr_full;
};

const std::string JSON_TYPE_ATTRIBUTE = "type";
const std::string JSON_VALUE_ATTRIBUTE = "value";
const std::string DYNAMIC_TYPE_ATTRIBUTE = "dynamicType";

// Function declaration

static std::string stob(bool b);

// Global variables

const std::string EMPTY_VAR_NAME = "Empty LocalVariable";
const std::string TEST_ATTRIBUTE_NAME = "Test Attribute Name";
const std::string TEST_ATTRIBUTE_VALUE = "Test Attribute Value";

const std::string BOOL_TYPE = R"RAW({"type":"bool"})RAW";
const std::string UINT64_TYPE = R"RAW({"type":"uint64"})RAW";
const std::string FLOAT32_TYPE = R"RAW({"type":"float32"})RAW";

const sup::dto::boolean BOOL_VALUE = true;
const std::string BOOL_VALUE_STR = stob(BOOL_VALUE);
const sup::dto::uint64 UINT64_VALUE = 98765;
const std::string UINT64_VALUE_STR = std::to_string(UINT64_VALUE);
const sup::dto::float32 FLOAT32_VALUE = 0.5772f;
const std::string FLOAT32_VALUE_STR = std::to_string(FLOAT32_VALUE);

// Function definition

TEST_F(LocalVariableTest, DefaultConstructed)
{
  // Test default constructed
  EXPECT_EQ(empty_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(empty_var.GetName().empty());
  EXPECT_FALSE(empty_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_FALSE(empty_var.IsAvailable());
  EXPECT_NO_THROW(empty_var.Setup(ws));
  EXPECT_TRUE(empty_var.IsAvailable());

  // Test Get/SetName
  empty_var.SetName(EMPTY_VAR_NAME);
  EXPECT_TRUE(empty_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttributeString(Constants::NAME_ATTRIBUTE_NAME));

  // Test GetValue
  sup::dto::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Test SetValue: should pass even if both are unitialized.
  EXPECT_TRUE(empty_var.SetValue(any_value));
}

TEST_F(LocalVariableTest, Setup)
{
  // Setup without type/value attributes succeeds
  LocalVariable var_no_attr{};
  EXPECT_NO_THROW(var_no_attr.Setup(ws));

  // Setup with only type attribute succeeds if parsing was successful
  LocalVariable var_type_ok{};
  EXPECT_TRUE(var_type_ok.AddAttribute("type", BOOL_TYPE));
  EXPECT_NO_THROW(var_type_ok.Setup(ws));

  // Setup with type and value attributea succeeds if parsing was successful
  LocalVariable var_val_ok{};
  EXPECT_TRUE(var_val_ok.AddAttribute("type", BOOL_TYPE));
  EXPECT_TRUE(var_val_ok.AddAttribute("value", "true"));
  EXPECT_NO_THROW(var_val_ok.Setup(ws));

  // Setup with only type attribute throws if parsing failed
  LocalVariable var_type_not_ok{};
  EXPECT_TRUE(var_type_not_ok.AddAttribute("type", "not_a_type"));
  EXPECT_THROW(var_type_not_ok.Setup(ws), VariableSetupException);

  // Setup with type and value attributea throws if parsing failed
  LocalVariable var_val_not_ok{};
  EXPECT_TRUE(var_val_not_ok.AddAttribute("type", BOOL_TYPE));
  EXPECT_TRUE(var_val_not_ok.AddAttribute("value", "on"));
  EXPECT_THROW(var_val_not_ok.Setup(ws), VariableSetupException);

  // Setup with only value attribute fails
  LocalVariable var_value_only{};
  EXPECT_TRUE(var_value_only.AddAttribute("value", "some_val"));
  EXPECT_THROW(var_value_only.Setup(ws), VariableSetupException);
}

TEST_F(LocalVariableTest, AddAttribute)
{
  // Test AddAttribute
  EXPECT_FALSE(empty_var.HasAttribute(TEST_ATTRIBUTE_NAME));
  EXPECT_TRUE(empty_var.AddAttribute(TEST_ATTRIBUTE_NAME, TEST_ATTRIBUTE_VALUE));
  EXPECT_TRUE(empty_var.HasAttribute(TEST_ATTRIBUTE_NAME));
  EXPECT_EQ(empty_var.GetAttributeString(TEST_ATTRIBUTE_NAME), TEST_ATTRIBUTE_VALUE);
}

TEST_F(LocalVariableTest, AddAttributesPartial)
{
  // Preconditions
  EXPECT_FALSE(empty_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_FALSE(empty_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  sup::dto::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Add attributes
  EXPECT_TRUE(empty_var.AddAttributes(attr_partial));
  EXPECT_NO_THROW(empty_var.Setup(ws));

  // Post conditions
  EXPECT_TRUE(empty_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(empty_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttributeString(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_EQ(empty_var.GetAttributeString(JSON_TYPE_ATTRIBUTE), UINT64_TYPE);
  EXPECT_TRUE(empty_var.GetValue(any_value));  // zero initialized
  EXPECT_EQ(any_value, static_cast<sup::dto::uint64>(0));
  sup::dto::AnyValue val(sup::dto::UnsignedInteger64Type);
  sup::dto::uint64 val_n = 5555ul;
  val = val_n;
  EXPECT_TRUE(empty_var.SetValue(val));
  EXPECT_TRUE(empty_var.GetValue(any_value));
  EXPECT_EQ(any_value, val_n);
}

TEST_F(LocalVariableTest, AddAttributesFull)
{
  // Preconditions
  EXPECT_FALSE(empty_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_FALSE(empty_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  EXPECT_FALSE(empty_var.HasAttribute(JSON_VALUE_ATTRIBUTE));
  EXPECT_NO_THROW(empty_var.Setup(ws));
  sup::dto::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Add attributes
  EXPECT_TRUE(empty_var.AddAttributes(attr_full));
  EXPECT_NO_THROW(empty_var.Setup(ws));

  // Get attributes
  EXPECT_EQ(empty_var.GetStringAttributes(), attr_full);

  // Post conditions
  EXPECT_TRUE(empty_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(empty_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  EXPECT_TRUE(empty_var.HasAttribute(JSON_VALUE_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttributeString(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_EQ(empty_var.GetAttributeString(JSON_TYPE_ATTRIBUTE), UINT64_TYPE);
  EXPECT_EQ(empty_var.GetAttributeString(JSON_VALUE_ATTRIBUTE), UINT64_VALUE_STR);
  EXPECT_TRUE(empty_var.GetValue(any_value));
  auto val = any_value.As<sup::dto::uint64>();
  EXPECT_EQ(val, UINT64_VALUE);
}

TEST_F(LocalVariableTest, NotifyCallback)
{
  sup::dto::int32 value = 0;
  LocalVariable int32_var{};
  EXPECT_TRUE(int32_var.AddAttribute(JSON_TYPE_ATTRIBUTE, R"RAW({"type":"int32"})RAW"));
  EXPECT_TRUE(int32_var.AddAttribute(JSON_VALUE_ATTRIBUTE, std::to_string(value)));
  int32_var.SetNotifyCallback(
    [&value](const sup::dto::AnyValue& val, bool)
    {
      value = val.As<sup::dto::int32>();
    });
  EXPECT_NO_THROW(int32_var.Setup(ws));
  sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type);
  new_value = 1234;
  EXPECT_TRUE(int32_var.SetValue(new_value));
  EXPECT_EQ(value, 1234);
}

TEST_F(LocalVariableTest, BooleanType)
{
  EXPECT_EQ(bool_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(bool_var.GetName().empty());
  EXPECT_FALSE(bool_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(bool_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  EXPECT_EQ(bool_var.GetAttributeString(JSON_TYPE_ATTRIBUTE), BOOL_TYPE);
  EXPECT_NO_THROW(bool_var.Setup(ws));
  sup::dto::AnyValue any_value;
  EXPECT_TRUE(bool_var.GetValue(any_value));
  bool b = any_value.As<sup::dto::boolean>();
  EXPECT_EQ(b, BOOL_VALUE);

  // Set compatible value
  sup::dto::AnyValue compatible(sup::dto::BooleanType);
  compatible = false;
  EXPECT_TRUE(bool_var.SetValue(compatible));
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value.As<sup::dto::boolean>();
  EXPECT_EQ(b, false);

  // Set incompatible value
  sup::dto::AnyValue incompatible(sup::dto::StringType, "incompatible");
  EXPECT_FALSE(bool_var.SetValue(incompatible));
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value.As<sup::dto::boolean>();
  EXPECT_EQ(b, false);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(bool_var.GetValue(incompatible));
  std::string val = incompatible.As<std::string>();
  EXPECT_EQ(val, "incompatible");  // unchanged
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value.As<sup::dto::boolean>();
  EXPECT_EQ(b, false);  // unchanged
}

TEST_F(LocalVariableTest, UnsignedInteger64Type)
{
  EXPECT_EQ(uint64_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(uint64_var.GetName().empty());
  EXPECT_FALSE(uint64_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(uint64_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  EXPECT_EQ(uint64_var.GetAttributeString(JSON_TYPE_ATTRIBUTE), UINT64_TYPE);
  EXPECT_NO_THROW(uint64_var.Setup(ws));
  sup::dto::AnyValue any_value;
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  sup::dto::uint64 val = any_value.As<sup::dto::uint64>();
  EXPECT_EQ(val, UINT64_VALUE);

  // Set compatible value
  sup::dto::uint64 val_2 = 244998ul;
  sup::dto::AnyValue compatible(sup::dto::UnsignedInteger64Type);
  compatible = val_2;
  EXPECT_TRUE(uint64_var.SetValue(compatible));
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value.As<sup::dto::uint64>();
  EXPECT_EQ(val, val_2);

  // Set incompatible value
  sup::dto::AnyValue incompatible(sup::dto::SignedInteger8Type);
  sup::dto::int8 inc_val = -18;
  incompatible = inc_val;
  EXPECT_FALSE(uint64_var.SetValue(incompatible));
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value.As<sup::dto::uint64>();
  EXPECT_EQ(val, val_2);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(uint64_var.GetValue(incompatible));
  sup::dto::int8 val_3 = incompatible.As<sup::dto::int8>();
  EXPECT_EQ(val_3, inc_val);  // unchanged
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value.As<sup::dto::uint64>();
  EXPECT_EQ(val, val_2);  // unchanged
}

TEST_F(LocalVariableTest, Float32Type)
{
  EXPECT_EQ(float32_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(float32_var.GetName().empty());
  EXPECT_FALSE(float32_var.HasAttribute(Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(float32_var.HasAttribute(JSON_TYPE_ATTRIBUTE));
  EXPECT_EQ(float32_var.GetAttributeString(JSON_TYPE_ATTRIBUTE), FLOAT32_TYPE);
  EXPECT_NO_THROW(float32_var.Setup(ws));
  sup::dto::AnyValue any_value;
  EXPECT_TRUE(float32_var.GetValue(any_value));
  sup::dto::float32 val = any_value.As<sup::dto::float32>();
  EXPECT_EQ(val, FLOAT32_VALUE);

  // Set compatible value
  sup::dto::float32 val_2 = 2.718f;
  sup::dto::AnyValue compatible(sup::dto::Float32Type);
  compatible = val_2;
  EXPECT_TRUE(float32_var.SetValue(compatible));
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value.As<sup::dto::float32>();
  EXPECT_EQ(val, val_2);

  // Set incompatible value
  std::string inc_val = "incompatible";
  sup::dto::AnyValue incompatible(sup::dto::StringType, inc_val);
  EXPECT_FALSE(float32_var.SetValue(incompatible));
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value.As<sup::dto::float32>();
  EXPECT_EQ(val, val_2);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(float32_var.GetValue(incompatible));
  std::string val_3 = incompatible.As<std::string>();
  EXPECT_EQ(val_3, inc_val);  // unchanged
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value.As<sup::dto::float32>();
  EXPECT_EQ(val, val_2);  // unchanged
}

TEST_F(LocalVariableTest, IllegalTypeChange)
{
  SetupVariables();
  {
    // Assign empty value
    sup::dto::AnyValue value{};
    EXPECT_TRUE(empty_var.SetValue(value));
    EXPECT_FALSE(bool_var.SetValue(value));
    EXPECT_FALSE(uint64_var.SetValue(value));
    EXPECT_FALSE(float32_var.SetValue(value));
  }
  {
    // Assign wrongly typed value (cannot be converted)
    sup::dto::AnyValue value = {{
      {"signed", {sup::dto::SignedInteger8Type, 1}},
      {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}
    }};
    EXPECT_TRUE(empty_var.SetValue(value));
    EXPECT_FALSE(bool_var.SetValue(value));
    EXPECT_FALSE(uint64_var.SetValue(value));
    EXPECT_FALSE(float32_var.SetValue(value));
  }
}

TEST_F(LocalVariableTest, LegalTypeChange)
{
  EXPECT_TRUE(empty_var.AddAttribute(DYNAMIC_TYPE_ATTRIBUTE, "true"));
  EXPECT_TRUE(bool_var.AddAttribute(DYNAMIC_TYPE_ATTRIBUTE, "true"));
  EXPECT_TRUE(uint64_var.AddAttribute(DYNAMIC_TYPE_ATTRIBUTE, "true"));
  EXPECT_TRUE(float32_var.AddAttribute(DYNAMIC_TYPE_ATTRIBUTE, "true"));
  SetupVariables();
  {
    // Assign empty value
    sup::dto::AnyValue value{};
    EXPECT_TRUE(empty_var.SetValue(value));
    EXPECT_TRUE(bool_var.SetValue(value));
    EXPECT_TRUE(uint64_var.SetValue(value));
    EXPECT_TRUE(float32_var.SetValue(value));
  }
  {
    // Assign differently typed value (cannot be converted)
    sup::dto::AnyValue value = {{
      {"signed", {sup::dto::SignedInteger8Type, 1}},
      {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}
    }};
    EXPECT_TRUE(empty_var.SetValue(value));
    EXPECT_TRUE(bool_var.SetValue(value));
    EXPECT_TRUE(uint64_var.SetValue(value));
    EXPECT_TRUE(float32_var.SetValue(value));
  }
  {
    // Assign differently typed value to leaf
    sup::dto::AnyValue value = {{
      {"signed", {sup::dto::SignedInteger8Type, 1}},
      {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}
    }};
    ASSERT_TRUE(empty_var.SetValue(value));
    EXPECT_TRUE(empty_var.SetValue(value, "signed"));
    // Reading back always keeps restrictions on assignment, so an empty variable is required here.
    sup::dto::AnyValue read_back{};
    EXPECT_TRUE(empty_var.GetValue(read_back));
    EXPECT_TRUE(read_back.HasField("signed.signed"));
  }
}

static std::string stob(bool b)
{
  std::stringstream str_s;
  str_s << std::boolalpha << b;
  return str_s.str();
}

LocalVariableTest::LocalVariableTest()
    : ws{}
    , empty_var{}, bool_var{}, uint64_var{}, float32_var{}, attr_partial{}, attr_full{}
{
  bool_var.AddAttribute(JSON_TYPE_ATTRIBUTE, BOOL_TYPE);
  bool_var.AddAttribute(JSON_VALUE_ATTRIBUTE, BOOL_VALUE_STR);
  uint64_var.AddAttribute(JSON_TYPE_ATTRIBUTE, UINT64_TYPE);
  uint64_var.AddAttribute(JSON_VALUE_ATTRIBUTE, UINT64_VALUE_STR);
  float32_var.AddAttribute(JSON_TYPE_ATTRIBUTE, FLOAT32_TYPE);
  float32_var.AddAttribute(JSON_VALUE_ATTRIBUTE, FLOAT32_VALUE_STR);

  attr_partial.emplace_back(Constants::NAME_ATTRIBUTE_NAME, EMPTY_VAR_NAME);
  attr_partial.emplace_back(JSON_TYPE_ATTRIBUTE, UINT64_TYPE);

  attr_full.emplace_back(Constants::NAME_ATTRIBUTE_NAME, EMPTY_VAR_NAME);
  attr_full.emplace_back(JSON_TYPE_ATTRIBUTE, UINT64_TYPE);
  attr_full.emplace_back(JSON_VALUE_ATTRIBUTE, UINT64_VALUE_STR);
}

LocalVariableTest::~LocalVariableTest() = default;

void LocalVariableTest::SetupVariables()
{
  empty_var.Setup(ws);
  bool_var.Setup(ws);
  uint64_var.Setup(ws);
  float32_var.Setup(ws);
}
