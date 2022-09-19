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

#include <sup/sequencer/variables/local_variable.h>

#include <sup/sequencer/log.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <future>
#include <sstream>

using namespace sup::sequencer;

class LocalVariableTest : public ::testing::Test
{
protected:
  LocalVariableTest();
  virtual ~LocalVariableTest();

  LocalVariable empty_var;
  LocalVariable bool_var;
  LocalVariable uint64_var;
  LocalVariable float32_var;
  AttributeMap attr_partial;
  AttributeMap attr_full;
};

// Function declaration

static std::string stob(bool b);

// Global variables

static const bool kLogToStdOut = (log::SetStdOut(), true);

static const std::string EMPTY_VAR_NAME = "Empty LocalVariable";
static const std::string TEST_ATTRIBUTE_NAME = "Test Attribute Name";
static const std::string TEST_ATTRIBUTE_VALUE = "Test Attribute Value";

static const std::string BOOL_TYPE = R"RAW({"type":"bool"})RAW";
static const std::string UINT64_TYPE = R"RAW({"type":"uint64"})RAW";
static const std::string FLOAT32_TYPE = R"RAW({"type":"float32"})RAW";

static const sup::dto::boolean BOOL_VALUE = true;
static const std::string BOOL_VALUE_STR = stob(BOOL_VALUE);
static const sup::dto::uint64 UINT64_VALUE = 98765;
static const std::string UINT64_VALUE_STR = std::to_string(UINT64_VALUE);
static const sup::dto::float32 FLOAT32_VALUE = 0.5772f;
static const std::string FLOAT32_VALUE_STR = std::to_string(FLOAT32_VALUE);

// Function definition

TEST_F(LocalVariableTest, DefaultConstructed)
{
  // Test default constructed
  EXPECT_EQ(empty_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(empty_var.GetName().empty());
  EXPECT_FALSE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_NO_THROW(empty_var.Setup());

  // Test Get/SetName
  empty_var.SetName(EMPTY_VAR_NAME);
  EXPECT_TRUE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttribute(attributes::NAME_ATTRIBUTE));

  // Test GetValue
  sup::dto::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Test SetValue: should pass even if both are unitialized.
  EXPECT_TRUE(empty_var.SetValue(any_value));
}

TEST_F(LocalVariableTest, AddAttribute)
{
  // Test AddAttribute
  EXPECT_FALSE(empty_var.HasAttribute(TEST_ATTRIBUTE_NAME));
  EXPECT_TRUE(empty_var.AddAttribute(TEST_ATTRIBUTE_NAME, TEST_ATTRIBUTE_VALUE));
  EXPECT_TRUE(empty_var.HasAttribute(TEST_ATTRIBUTE_NAME));
  EXPECT_EQ(empty_var.GetAttribute(TEST_ATTRIBUTE_NAME), TEST_ATTRIBUTE_VALUE);
}

TEST_F(LocalVariableTest, AddAttributesPartial)
{
  // Preconditions
  EXPECT_FALSE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_FALSE(empty_var.HasAttribute(LocalVariable::JSON_TYPE));
  sup::dto::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Add attributes
  EXPECT_TRUE(empty_var.AddAttributes(attr_partial));
  EXPECT_NO_THROW(empty_var.Setup());

  // Post conditions
  EXPECT_TRUE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(empty_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetAttribute(LocalVariable::JSON_TYPE), UINT64_TYPE);
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
  EXPECT_FALSE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_FALSE(empty_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_FALSE(empty_var.HasAttribute(LocalVariable::JSON_VALUE));
  EXPECT_NO_THROW(empty_var.Setup());
  sup::dto::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Add attributes
  EXPECT_TRUE(empty_var.AddAttributes(attr_full));
  EXPECT_NO_THROW(empty_var.Setup());

  // Get attributes
  EXPECT_EQ(empty_var.GetAttributes().GetAttributeNames(), attr_full.GetAttributeNames());

  // Post conditions
  EXPECT_TRUE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(empty_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_TRUE(empty_var.HasAttribute(LocalVariable::JSON_VALUE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetAttribute(LocalVariable::JSON_TYPE), UINT64_TYPE);
  EXPECT_EQ(empty_var.GetAttribute(LocalVariable::JSON_VALUE), UINT64_VALUE_STR);
  EXPECT_TRUE(empty_var.GetValue(any_value));
  sup::dto::uint64 val = any_value;
  EXPECT_EQ(val, UINT64_VALUE);
}

TEST_F(LocalVariableTest, NotifyCallback)
{
  sup::dto::int32 value = 0;
  LocalVariable int32_var{};
  EXPECT_TRUE(int32_var.AddAttribute(LocalVariable::JSON_TYPE, R"RAW({"type":"int32"})RAW"));
  EXPECT_TRUE(int32_var.AddAttribute(LocalVariable::JSON_VALUE, std::to_string(value)));
  int32_var.SetNotifyCallback(
    [&value](const sup::dto::AnyValue& val)
    {
      value = val;
    });
  EXPECT_NO_THROW(int32_var.Setup());
  sup::dto::AnyValue new_value(sup::dto::SignedInteger32Type);
  new_value = 1234;
  EXPECT_TRUE(int32_var.SetValue(new_value));
  EXPECT_EQ(value, 1234);
}

TEST_F(LocalVariableTest, BooleanType)
{
  EXPECT_EQ(bool_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(bool_var.GetName().empty());
  EXPECT_FALSE(bool_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(bool_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_EQ(bool_var.GetAttribute(LocalVariable::JSON_TYPE), BOOL_TYPE);
  EXPECT_NO_THROW(bool_var.Setup());
  sup::dto::AnyValue any_value;
  EXPECT_TRUE(bool_var.GetValue(any_value));
  bool b = any_value;
  EXPECT_EQ(b, BOOL_VALUE);

  // Set compatible value
  sup::dto::AnyValue compatible(sup::dto::BooleanType);
  compatible = false;
  EXPECT_TRUE(bool_var.SetValue(compatible));
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value;
  EXPECT_EQ(b, false);

  // Set incompatible value
  sup::dto::AnyValue incompatible(sup::dto::Float64Type);
  incompatible = 3.14;
  EXPECT_FALSE(bool_var.SetValue(incompatible));
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value;
  EXPECT_EQ(b, false);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(bool_var.GetValue(incompatible));
  sup::dto::float64 val = incompatible;
  EXPECT_EQ(val, 3.14);  // unchanged
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value;
  EXPECT_EQ(b, false);  // unchanged
}

TEST_F(LocalVariableTest, UnsignedInteger64Type)
{
  EXPECT_EQ(uint64_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(uint64_var.GetName().empty());
  EXPECT_FALSE(uint64_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(uint64_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_EQ(uint64_var.GetAttribute(LocalVariable::JSON_TYPE), UINT64_TYPE);
  EXPECT_NO_THROW(uint64_var.Setup());
  sup::dto::AnyValue any_value;
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  sup::dto::uint64 val = any_value;
  EXPECT_EQ(val, UINT64_VALUE);

  // Set compatible value
  sup::dto::uint64 val_2 = 244998ul;
  sup::dto::AnyValue compatible(sup::dto::UnsignedInteger64Type);
  compatible = val_2;
  EXPECT_TRUE(uint64_var.SetValue(compatible));
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);

  // Set incompatible value
  sup::dto::AnyValue incompatible(sup::dto::SignedInteger8Type);
  sup::dto::int8 inc_val = -18;
  incompatible = inc_val;
  EXPECT_FALSE(uint64_var.SetValue(incompatible));
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(uint64_var.GetValue(incompatible));
  sup::dto::int8 val_3 = incompatible;
  EXPECT_EQ(val_3, inc_val);  // unchanged
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged
}

TEST_F(LocalVariableTest, Float32Type)
{
  EXPECT_EQ(float32_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(float32_var.GetName().empty());
  EXPECT_FALSE(float32_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(float32_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_EQ(float32_var.GetAttribute(LocalVariable::JSON_TYPE), FLOAT32_TYPE);
  EXPECT_NO_THROW(float32_var.Setup());
  sup::dto::AnyValue any_value;
  EXPECT_TRUE(float32_var.GetValue(any_value));
  sup::dto::float32 val = any_value;
  EXPECT_EQ(val, FLOAT32_VALUE);

  // Set compatible value
  sup::dto::float32 val_2 = 2.718f;
  sup::dto::AnyValue compatible(sup::dto::Float32Type);
  compatible = val_2;
  EXPECT_TRUE(float32_var.SetValue(compatible));
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);

  // Set incompatible value
  sup::dto::AnyValue incompatible(sup::dto::SignedInteger8Type);
  sup::dto::int8 inc_val = -18;
  incompatible = inc_val;
  EXPECT_FALSE(float32_var.SetValue(incompatible));
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(float32_var.GetValue(incompatible));
  sup::dto::int8 val_3 = incompatible;
  EXPECT_EQ(val_3, inc_val);  // unchanged
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged
}

// TEST_F(LocalVariableTest, WaitForSuccess)
// {
//   std::promise<void> ready;
//   auto wait_activity = [this, &ready]()
//   {
//     ready.set_value();
//     return uint64_var.WaitFor(3.0);
//   };
//   auto result = std::async(std::launch::async, wait_activity);
//   ready.get_future().wait();
//   std::this_thread::sleep_for(std::chrono::milliseconds(250));
//   sup::dto::AnyValue val(sup::dto::UnsignedInteger64Type);
//   val = 5;
//   EXPECT_TRUE(uint64_var.SetValue(val));
//   EXPECT_TRUE(result.get());
// }

// TEST_F(LocalVariableTest, WaitForTimeout)
// {
//   auto wait_activity = [this]()
//   {
//     return uint64_var.WaitFor(0.1);
//   };
//   auto result = std::async(std::launch::async, wait_activity);
//   EXPECT_FALSE(result.get());
// }

static std::string stob(bool b)
{
  std::stringstream str_s;
  str_s << std::boolalpha << b;
  return str_s.str();
}

LocalVariableTest::LocalVariableTest()
    : empty_var{}, bool_var{}, uint64_var{}, float32_var{}, attr_partial{}, attr_full{}
{
  bool_var.AddAttribute(LocalVariable::JSON_TYPE, BOOL_TYPE);
  bool_var.AddAttribute(LocalVariable::JSON_VALUE, BOOL_VALUE_STR);
  uint64_var.AddAttribute(LocalVariable::JSON_TYPE, UINT64_TYPE);
  uint64_var.AddAttribute(LocalVariable::JSON_VALUE, UINT64_VALUE_STR);
  float32_var.AddAttribute(LocalVariable::JSON_TYPE, FLOAT32_TYPE);
  float32_var.AddAttribute(LocalVariable::JSON_VALUE, FLOAT32_VALUE_STR);

  attr_partial.AddAttribute(attributes::NAME_ATTRIBUTE, EMPTY_VAR_NAME);
  attr_partial.AddAttribute(LocalVariable::JSON_TYPE, UINT64_TYPE);

  attr_full.AddAttribute(attributes::NAME_ATTRIBUTE, EMPTY_VAR_NAME);
  attr_full.AddAttribute(LocalVariable::JSON_TYPE, UINT64_TYPE);
  attr_full.AddAttribute(LocalVariable::JSON_VALUE, UINT64_VALUE_STR);
}

LocalVariableTest::~LocalVariableTest() = default;
