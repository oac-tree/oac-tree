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

#include <sup/sequencer/attribute_handler.h>
#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/exceptions.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

const std::string kStrAttrName = "str_attr_name";
const std::string kDoubleAttrName = "double_attr_name";
const std::string kBoolAttrName = "bool_attr_name";

const std::string kStrAttrValue = "str_attr_value";
const std::string kDoubleAttrValue = "3.14";
const std::string kBoolAttrValue = "true";

class AttributeHandlerTest : public ::testing::Test
{
protected:
  AttributeHandlerTest();
  virtual ~AttributeHandlerTest();
};

TEST_F(AttributeHandlerTest, AttributeDefinitions)
{
  {
    // Successful addition and failed one
    AttributeHandler handler;
    auto attr_defs = handler.GetAttributeDefinitions();
    EXPECT_EQ(attr_defs.size(), 0);
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kStrAttrName, sup::dto::StringType));
    attr_defs = handler.GetAttributeDefinitions();
    EXPECT_EQ(attr_defs.size(), 1);
    EXPECT_THROW(handler.AddAttributeDefinition(kStrAttrName, sup::dto::BooleanType),
                 InvalidOperationException);
    attr_defs = handler.GetAttributeDefinitions();
    ASSERT_EQ(attr_defs.size(), 1);
    EXPECT_EQ(attr_defs[0].GetName(), kStrAttrName);
    EXPECT_EQ(attr_defs[0].GetType(), sup::dto::StringType);
    EXPECT_FALSE(attr_defs[0].IsMandatory());
  }
  {
    // Successful addition of mandatory attribute
    AttributeHandler handler;
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kStrAttrName, sup::dto::StringType).SetMandatory());
    auto attr_defs = handler.GetAttributeDefinitions();
    ASSERT_EQ(attr_defs.size(), 1);
    EXPECT_EQ(attr_defs[0].GetName(), kStrAttrName);
    EXPECT_EQ(attr_defs[0].GetType(), sup::dto::StringType);
    EXPECT_TRUE(attr_defs[0].IsMandatory());
  }
}

TEST_F(AttributeHandlerTest, SetStringAttribute)
{
  AttributeHandler handler;
  auto str_attributes = handler.GetStringAttributes();
  EXPECT_EQ(str_attributes.size(), 0);
  EXPECT_NO_THROW(handler.AddStringAttribute(kStrAttrName, kBoolAttrValue));
  EXPECT_TRUE(handler.HasStringAttribute(kStrAttrName));
  str_attributes = handler.GetStringAttributes();
  ASSERT_EQ(str_attributes.size(), 1);
  auto [attr_name, attr_value] = str_attributes[0];
  EXPECT_EQ(attr_name, kStrAttrName);
  EXPECT_EQ(attr_value, kBoolAttrValue);
  EXPECT_NO_THROW(handler.SetStringAttribute(kStrAttrName, kStrAttrValue));
  str_attributes = handler.GetStringAttributes();
  ASSERT_EQ(str_attributes.size(), 1);
  auto [attr_name2, attr_value2] = str_attributes[0];
  EXPECT_EQ(attr_name2, kStrAttrName);
  EXPECT_EQ(attr_value2, kStrAttrValue);
}

TEST_F(AttributeHandlerTest, GetValueAs)
{
  AttributeHandler handler;
  EXPECT_NO_THROW(handler.AddStringAttribute(kStrAttrName, kStrAttrValue));
  EXPECT_TRUE(handler.ValidateAttributes());
  std::string str_val;
  EXPECT_TRUE(handler.GetValueAs(kStrAttrName, str_val));
  EXPECT_FALSE(handler.GetValueAs(kDoubleAttrName, str_val));
  double double_val;
  EXPECT_FALSE(handler.GetValueAs(kStrAttrName, double_val));
}

TEST_F(AttributeHandlerTest, MandatoryAttributes)
{
  // Successful addition and failed one
  AttributeHandler handler;
  EXPECT_NO_THROW(
    handler.AddAttributeDefinition(kStrAttrName, sup::dto::StringType).SetMandatory());
  auto attr_defs = handler.GetAttributeDefinitions();
  EXPECT_EQ(attr_defs.size(), 1);
  EXPECT_NO_THROW(
    handler.AddAttributeDefinition(kDoubleAttrName, sup::dto::Float64Type).SetMandatory());
  attr_defs = handler.GetAttributeDefinitions();
  EXPECT_EQ(attr_defs.size(), 2);
  EXPECT_NO_THROW(
    handler.AddAttributeDefinition(kBoolAttrName, sup::dto::BooleanType).SetMandatory());
  attr_defs = handler.GetAttributeDefinitions();
  EXPECT_EQ(attr_defs.size(), 3);
  // Three constraints fail due to three missing mandatory attributes
  EXPECT_FALSE(handler.ValidateAttributes());
  auto failed_constraints = handler.GetFailedConstraints();
  EXPECT_EQ(failed_constraints.size(), 3);
  // Two constraints fail due to two missing mandatory attributes
  EXPECT_TRUE(handler.AddStringAttribute(kStrAttrName, kStrAttrValue));
  EXPECT_FALSE(handler.ValidateAttributes());
  failed_constraints = handler.GetFailedConstraints();
  EXPECT_EQ(failed_constraints.size(), 2);
  // Two constraints fail: missing mandatory attribute and failed to parse other
  EXPECT_TRUE(handler.AddStringAttribute(kDoubleAttrName, "cannot_be_parsed"));
  EXPECT_FALSE(handler.ValidateAttributes());
  failed_constraints = handler.GetFailedConstraints();
  EXPECT_EQ(failed_constraints.size(), 2);
  // One constraint fails due to one missing mandatory attribute
  EXPECT_NO_THROW(handler.SetStringAttribute(kDoubleAttrName, kDoubleAttrValue));
  EXPECT_FALSE(handler.ValidateAttributes());
  failed_constraints = handler.GetFailedConstraints();
  EXPECT_EQ(failed_constraints.size(), 1);
  // All constraints pass
  EXPECT_TRUE(handler.AddStringAttribute(kBoolAttrName, kBoolAttrValue));
  EXPECT_TRUE(handler.ValidateAttributes());
  failed_constraints = handler.GetFailedConstraints();
  EXPECT_EQ(failed_constraints.size(), 0);
  // Inspect anyvalue values
  sup::dto::AnyValue str_val;
  EXPECT_TRUE(handler.GetValue(kStrAttrName, str_val));
  EXPECT_FALSE(sup::dto::IsEmptyValue(str_val));
  EXPECT_EQ(str_val.GetType(), sup::dto::StringType);
  EXPECT_EQ(str_val.As<std::string>(), kStrAttrValue);
  sup::dto::AnyValue double_val;
  EXPECT_TRUE(handler.GetValue(kDoubleAttrName, double_val));
  EXPECT_FALSE(sup::dto::IsEmptyValue(double_val));
  EXPECT_EQ(double_val.GetType(), sup::dto::Float64Type);
  EXPECT_EQ(double_val.As<double>(), 3.14);
  sup::dto::AnyValue bool_val;
  EXPECT_TRUE(handler.GetValue(kBoolAttrName, bool_val));
  EXPECT_FALSE(sup::dto::IsEmptyValue(bool_val));
  EXPECT_EQ(bool_val.GetType(), sup::dto::BooleanType);
  EXPECT_EQ(bool_val.As<bool>(), true);
}

TEST_F(AttributeHandlerTest, NonDefinedAttributes)
{
  // Attributes without definition are of type string by default
  AttributeHandler handler;
  EXPECT_TRUE(handler.AddStringAttribute(kBoolAttrName, kBoolAttrValue));
  EXPECT_TRUE(handler.ValidateAttributes());
  auto failed_constraints = handler.GetFailedConstraints();
  EXPECT_EQ(failed_constraints.size(), 0);
  sup::dto::AnyValue bool_val;
  EXPECT_TRUE(handler.GetValue(kBoolAttrName, bool_val));
  EXPECT_FALSE(sup::dto::IsEmptyValue(bool_val));
  EXPECT_EQ(bool_val.GetType(), sup::dto::StringType);
  EXPECT_EQ(bool_val.As<std::string>(), kBoolAttrValue);
}

TEST_F(AttributeHandlerTest, ComplexConstraint)
{
  {
    AttributeHandler handler;
    // Add optional attributes
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kStrAttrName, sup::dto::StringType));
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kDoubleAttrName, sup::dto::Float64Type));
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kBoolAttrName, sup::dto::BooleanType));
    // Add constraint: the string attribute has to exist XOR both the double and boolean attribute
    // need to exist
    EXPECT_NO_THROW(handler.AddConstraint(
      MakeConstraint<Xor>(MakeConstraint<Exists>(kStrAttrName),
                             MakeConstraint<And>(MakeConstraint<Exists>(kDoubleAttrName),
                                                  MakeConstraint<Exists>(kBoolAttrName)))));
    EXPECT_FALSE(handler.ValidateAttributes());
    auto failed_constraints = handler.GetFailedConstraints();
    EXPECT_EQ(failed_constraints.size(), 1);
    // Adding the string attribute results in no failed constraints
    EXPECT_TRUE(handler.AddStringAttribute(kStrAttrName, kStrAttrValue));
    EXPECT_TRUE(handler.ValidateAttributes());
    failed_constraints = handler.GetFailedConstraints();
    EXPECT_EQ(failed_constraints.size(), 0);
  }
  {
    AttributeHandler handler;
    // Add optional attributes
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kStrAttrName, sup::dto::StringType));
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kDoubleAttrName, sup::dto::Float64Type));
    EXPECT_NO_THROW(handler.AddAttributeDefinition(kBoolAttrName, sup::dto::BooleanType));
    // Add constraint: the string attribute has to exist XOR both the double and boolean attribute
    // need to exist
    EXPECT_NO_THROW(handler.AddConstraint(
      MakeConstraint<Xor>(MakeConstraint<Exists>(kStrAttrName),
                             MakeConstraint<And>(MakeConstraint<Exists>(kDoubleAttrName),
                                                  MakeConstraint<Exists>(kBoolAttrName)))));
    EXPECT_FALSE(handler.ValidateAttributes());
    auto failed_constraints = handler.GetFailedConstraints();
    EXPECT_EQ(failed_constraints.size(), 1);
    // Adding only the double attribute results in a failed constraint
    EXPECT_TRUE(handler.AddStringAttribute(kDoubleAttrName, kDoubleAttrValue));
    EXPECT_FALSE(handler.ValidateAttributes());
    failed_constraints = handler.GetFailedConstraints();
    EXPECT_EQ(failed_constraints.size(), 1);
    // Adding the boolean attribute too makes it successful
    EXPECT_TRUE(handler.AddStringAttribute(kBoolAttrName, kBoolAttrValue));
    EXPECT_TRUE(handler.ValidateAttributes());
    failed_constraints = handler.GetFailedConstraints();
    EXPECT_EQ(failed_constraints.size(), 0);
  }
}

AttributeHandlerTest::AttributeHandlerTest() = default;

AttributeHandlerTest::~AttributeHandlerTest() = default;
