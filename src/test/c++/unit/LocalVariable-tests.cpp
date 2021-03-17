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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files

#include <gtest/gtest.h> // Google test framework
#include <common/log-api.h> // Syslog wrapper routines
#include <algorithm>
#include <sstream>

// Local header files

#include "LocalVariable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

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

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

static const std::string EMPTY_VAR_NAME = "Empty LocalVariable";
static const std::string TEST_ATTRIBUTE_NAME = "Test Attribute Name";
static const std::string TEST_ATTRIBUTE_VALUE = "Test Attribute Value";

static const std::string BOOL_TYPE = R"RAW({"type":"bool"})RAW";
static const std::string UINT64_TYPE = R"RAW({"type":"uint64"})RAW";
static const std::string FLOAT32_TYPE = R"RAW({"type":"float32"})RAW";

static const ::ccs::types::boolean BOOL_VALUE = true;
static const std::string BOOL_VALUE_STR = stob(BOOL_VALUE);
static const ::ccs::types::uint64 UINT64_VALUE = 98765;
static const std::string UINT64_VALUE_STR = std::to_string(UINT64_VALUE);
static const ::ccs::types::float32 FLOAT32_VALUE = 0.5772f;
static const std::string FLOAT32_VALUE_STR = std::to_string(FLOAT32_VALUE);

// Function definition

TEST_F(LocalVariableTest, DefaultConstructed)
{
  // Test default constructed
  EXPECT_EQ(empty_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(empty_var.GetName().empty());
  EXPECT_FALSE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));

  // Test Get/SetName
  empty_var.SetName(EMPTY_VAR_NAME);
  EXPECT_TRUE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttribute(attributes::NAME_ATTRIBUTE));

  // Test GetValue
  ::ccs::types::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Test SetValue
  EXPECT_FALSE(empty_var.SetValue(any_value));
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
  ::ccs::types::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Add attributes
  EXPECT_TRUE(empty_var.AddAttributes(attr_partial));

  // Post conditions
  EXPECT_TRUE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(empty_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetAttribute(LocalVariable::JSON_TYPE), UINT64_TYPE);
  EXPECT_FALSE(empty_var.GetValue(any_value));
  ::ccs::types::AnyValue val(::ccs::types::UnsignedInteger64);
  ::ccs::types::uint64 val_n = 5555ul;
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
  ::ccs::types::AnyValue any_value;
  EXPECT_FALSE(empty_var.GetValue(any_value));

  // Add attributes
  EXPECT_TRUE(empty_var.AddAttributes(attr_full));

  // Post conditions
  EXPECT_TRUE(empty_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(empty_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_TRUE(empty_var.HasAttribute(LocalVariable::JSON_VALUE));
  EXPECT_EQ(empty_var.GetName(), EMPTY_VAR_NAME);
  EXPECT_EQ(empty_var.GetName(), empty_var.GetAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(empty_var.GetAttribute(LocalVariable::JSON_TYPE), UINT64_TYPE);
  EXPECT_EQ(empty_var.GetAttribute(LocalVariable::JSON_VALUE), UINT64_VALUE_STR);
  EXPECT_TRUE(empty_var.GetValue(any_value));
  ::ccs::types::uint64 val = any_value;
  EXPECT_EQ(val, UINT64_VALUE);
}

TEST_F(LocalVariableTest, BooleanType)
{
  EXPECT_EQ(bool_var.GetType(), LocalVariable::Type);
  EXPECT_TRUE(bool_var.GetName().empty());
  EXPECT_FALSE(bool_var.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(bool_var.HasAttribute(LocalVariable::JSON_TYPE));
  EXPECT_EQ(bool_var.GetAttribute(LocalVariable::JSON_TYPE), BOOL_TYPE);
  ::ccs::types::AnyValue any_value;
  EXPECT_TRUE(bool_var.GetValue(any_value));
  bool b = any_value;
  EXPECT_EQ(b, BOOL_VALUE);

  // Set compatible value
  ::ccs::types::AnyValue compatible(::ccs::types::Boolean);
  compatible = false;
  EXPECT_TRUE(bool_var.SetValue(compatible));
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value;
  EXPECT_EQ(b, false);

  // Set incompatible value
  ::ccs::types::AnyValue incompatible(::ccs::types::Float64);
  incompatible = 3.14;
  EXPECT_FALSE(bool_var.SetValue(incompatible));
  EXPECT_TRUE(bool_var.GetValue(any_value));
  b = any_value;
  EXPECT_EQ(b, false);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(bool_var.GetValue(incompatible));
  ::ccs::types::float64 val = incompatible;
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
  ::ccs::types::AnyValue any_value;
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  ::ccs::types::uint64 val = any_value;
  EXPECT_EQ(val, UINT64_VALUE);

  // Set compatible value
  ::ccs::types::uint64 val_2 = 244998ul;
  ::ccs::types::AnyValue compatible(::ccs::types::UnsignedInteger64);
  compatible = val_2;
  EXPECT_TRUE(uint64_var.SetValue(compatible));
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);

  // Set incompatible value
  ::ccs::types::AnyValue incompatible(::ccs::types::SignedInteger8);
  ::ccs::types::int8 inc_val = -18;
  incompatible = inc_val;
  EXPECT_FALSE(uint64_var.SetValue(incompatible));
  EXPECT_TRUE(uint64_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(uint64_var.GetValue(incompatible));
  ::ccs::types::int8 val_3 = incompatible;
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
  ::ccs::types::AnyValue any_value;
  EXPECT_TRUE(float32_var.GetValue(any_value));
  ::ccs::types::float32 val = any_value;
  EXPECT_EQ(val, FLOAT32_VALUE);

  // Set compatible value
  ::ccs::types::float32 val_2 = 2.718f;
  ::ccs::types::AnyValue compatible(::ccs::types::Float32);
  compatible = val_2;
  EXPECT_TRUE(float32_var.SetValue(compatible));
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);

  // Set incompatible value
  ::ccs::types::AnyValue incompatible(::ccs::types::SignedInteger8);
  ::ccs::types::int8 inc_val = -18;
  incompatible = inc_val;
  EXPECT_FALSE(float32_var.SetValue(incompatible));
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged

  // Get incompatible value
  EXPECT_FALSE(float32_var.GetValue(incompatible));
  ::ccs::types::int8 val_3 = incompatible;
  EXPECT_EQ(val_3, inc_val);  // unchanged
  EXPECT_TRUE(float32_var.GetValue(any_value));
  val = any_value;
  EXPECT_EQ(val, val_2);  // unchanged
}

static std::string stob(bool b)
{
  std::stringstream str_s;
  str_s << std::boolalpha << b;
  return str_s.str();
}

LocalVariableTest::LocalVariableTest()
  : empty_var{}
  , bool_var{}
  , uint64_var{}
  , float32_var{}
  , attr_partial{}
  , attr_full{}
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

#undef LOG_ALTERN_SRC
