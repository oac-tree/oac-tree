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

// Global header files

#include <gtest/gtest.h>  // Google test framework

#include <algorithm>

// Local header files

#include "AttributeMap.h"

// Constants

// Type definition

using namespace sup::sequencer;

class AttributeMapTest : public ::testing::Test
{
protected:
  AttributeMapTest();
  virtual ~AttributeMapTest();

  AttributeMap attr_map_0;
  AttributeMap attr_map_1;
  AttributeMap attr_map_2;
};

// Function declaration

// Global variables

static const std::string NAME_VALUE = "TestName";
static const std::string DESCRIPTION_ATTRIBUTE = "Description";
static const std::string DESCRIPTION_VALUE = "Description of this testing attribute";
static const std::string VARIABLE_ATTRIBUTE = "MyVariable";

// Function definition

TEST_F(AttributeMapTest, DefaultConstructed)
{
  EXPECT_FALSE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));

  std::string empty_str;
  EXPECT_EQ(attr_map_0.GetAttribute(attributes::NAME_ATTRIBUTE), empty_str);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 0);
  auto attr_names = attr_map_0.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 0);

  int n = 0;
  for (const auto& attr : attr_map_0)
  {
    ++n;
  }
  EXPECT_EQ(n, 0);
}

TEST_F(AttributeMapTest, CopyConstructed)
{
  AttributeMap attr_map = attr_map_1;
  EXPECT_EQ(attr_map, attr_map_1);
  EXPECT_FALSE(attr_map != attr_map_1);
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), attr_map_1.GetNumberOfAttributes());

  for (const auto& attr : attr_map_1)
  {
    EXPECT_TRUE(attr_map.HasAttribute(attr.first));
    EXPECT_EQ(attr_map.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(AttributeMapTest, MoveConstructed)
{
  AttributeMap tmp_map = attr_map_1;  // make copy first to allow comparison later on
  AttributeMap attr_map = std::move(tmp_map);
  EXPECT_EQ(attr_map, attr_map_1);
  EXPECT_FALSE(attr_map != attr_map_1);
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), attr_map_1.GetNumberOfAttributes());

  for (const auto& attr : attr_map_1)
  {
    EXPECT_TRUE(attr_map.HasAttribute(attr.first));
    EXPECT_EQ(attr_map.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(AttributeMapTest, CopyAssigned)
{
  attr_map_0 = attr_map_1;
  EXPECT_EQ(attr_map_0, attr_map_1);
  EXPECT_FALSE(attr_map_0 != attr_map_1);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), attr_map_1.GetNumberOfAttributes());

  for (const auto& attr : attr_map_1)
  {
    EXPECT_TRUE(attr_map_0.HasAttribute(attr.first));
    EXPECT_EQ(attr_map_0.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(AttributeMapTest, MoveAssigned)
{
  AttributeMap tmp_map = attr_map_1;  // make copy first to allow comparison later on
  attr_map_0 = std::move(tmp_map);
  EXPECT_EQ(attr_map_0, attr_map_1);
  EXPECT_FALSE(attr_map_0 != attr_map_1);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), attr_map_1.GetNumberOfAttributes());

  for (const auto& attr : attr_map_1)
  {
    EXPECT_TRUE(attr_map_0.HasAttribute(attr.first));
    EXPECT_EQ(attr_map_0.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(AttributeMapTest, AddAttribute)
{
  // Add a name attribute
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 0);
  EXPECT_FALSE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_0.AddAttribute(attributes::NAME_ATTRIBUTE, NAME_VALUE));
  EXPECT_TRUE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(attr_map_0.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 1);
  auto attr_names = attr_map_0.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 1);
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), attributes::NAME_ATTRIBUTE),
            attr_names.end());
  int n = 0;
  for (auto attr : attr_map_0)
  {
    EXPECT_EQ(attr.first, attributes::NAME_ATTRIBUTE);
    EXPECT_EQ(attr.second, NAME_VALUE);
    ++n;
  }
  EXPECT_EQ(n, 1);

  // Add a description attribute
  EXPECT_FALSE(attr_map_0.HasAttribute(DESCRIPTION_ATTRIBUTE));
  EXPECT_TRUE(attr_map_0.AddAttribute(DESCRIPTION_ATTRIBUTE, DESCRIPTION_VALUE));
  EXPECT_TRUE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_0.HasAttribute(DESCRIPTION_ATTRIBUTE));
  EXPECT_EQ(attr_map_0.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_0.GetAttribute(DESCRIPTION_ATTRIBUTE), DESCRIPTION_VALUE);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 2);
  attr_names = attr_map_0.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 2);
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), attributes::NAME_ATTRIBUTE),
            attr_names.end());
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), DESCRIPTION_ATTRIBUTE),
            attr_names.end());
  n = 0;
  for (const auto& attr : attr_map_0)
  {
    ++n;
  }
  EXPECT_EQ(n, 2);

  // Try to overwrite the description attribute by using AddAttribute (should fail)
  std::string new_description = "New description";
  EXPECT_FALSE(attr_map_0.AddAttribute(DESCRIPTION_ATTRIBUTE, new_description));
  EXPECT_EQ(attr_map_0.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_0.GetAttribute(DESCRIPTION_ATTRIBUTE), DESCRIPTION_VALUE);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 2);
  attr_names = attr_map_0.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 2);
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), attributes::NAME_ATTRIBUTE),
            attr_names.end());
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), DESCRIPTION_ATTRIBUTE),
            attr_names.end());
  n = 0;
  for (const auto& attr : attr_map_0)
  {
    ++n;
  }
  EXPECT_EQ(n, 2);
}

TEST_F(AttributeMapTest, SetAttribute)
{
  // Set a name attribute
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 0);
  EXPECT_FALSE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  attr_map_0.SetAttribute(attributes::NAME_ATTRIBUTE, NAME_VALUE);
  EXPECT_TRUE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(attr_map_0.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 1);
  auto attr_names = attr_map_0.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 1);
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), attributes::NAME_ATTRIBUTE),
            attr_names.end());
  int n = 0;
  for (auto attr : attr_map_0)
  {
    EXPECT_EQ(attr.first, attributes::NAME_ATTRIBUTE);
    EXPECT_EQ(attr.second, NAME_VALUE);
    ++n;
  }
  EXPECT_EQ(n, 1);

  // Try to overwrite the description attribute by using SetAttribute (should succeed)
  std::string overwrite_val = "Overwritten";
  attr_map_0.SetAttribute(attributes::NAME_ATTRIBUTE, overwrite_val);
  EXPECT_TRUE(attr_map_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(attr_map_0.GetAttribute(attributes::NAME_ATTRIBUTE), overwrite_val);
  EXPECT_EQ(attr_map_0.GetNumberOfAttributes(), 1);
  attr_names = attr_map_0.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 1);
  EXPECT_NE(std::find(attr_names.begin(), attr_names.end(), attributes::NAME_ATTRIBUTE),
            attr_names.end());
  n = 0;
  for (const auto& attr : attr_map_0)
  {
    EXPECT_EQ(attr.first, attributes::NAME_ATTRIBUTE);
    EXPECT_EQ(attr.second, overwrite_val);
    ++n;
  }
  EXPECT_EQ(n, 1);
}

TEST_F(AttributeMapTest, Remove)
{
  EXPECT_EQ(attr_map_1.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(attr_map_1.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_1.HasAttribute(DESCRIPTION_ATTRIBUTE));

  std::string does_not_exist = "Nonexistent attribute name";
  EXPECT_FALSE(attr_map_1.Remove(does_not_exist));
  EXPECT_EQ(attr_map_1.GetNumberOfAttributes(), 2);

  EXPECT_TRUE(attr_map_1.Remove(DESCRIPTION_ATTRIBUTE));
  EXPECT_EQ(attr_map_1.GetNumberOfAttributes(), 1);
  EXPECT_FALSE(attr_map_1.HasAttribute(DESCRIPTION_ATTRIBUTE));
  EXPECT_TRUE(attr_map_1.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(attr_map_1.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);

  int n = 0;
  for (const auto& attr : attr_map_1)
  {
    EXPECT_EQ(attr.first, attributes::NAME_ATTRIBUTE);
    EXPECT_EQ(attr.second, NAME_VALUE);
    ++n;
  }
  EXPECT_EQ(n, 1);
}

TEST_F(AttributeMapTest, Clear)
{
  EXPECT_EQ(attr_map_1.GetNumberOfAttributes(), 2);
  attr_map_1.Clear();
  EXPECT_FALSE(attr_map_1.HasAttribute(attributes::NAME_ATTRIBUTE));

  std::string empty_str;
  EXPECT_EQ(attr_map_1.GetAttribute(attributes::NAME_ATTRIBUTE), empty_str);
  EXPECT_EQ(attr_map_1.GetNumberOfAttributes(), 0);
  auto attr_names = attr_map_1.GetAttributeNames();
  EXPECT_EQ(attr_names.size(), 0);

  int n = 0;
  for (const auto& attr : attr_map_1)
  {
    ++n;
  }
  EXPECT_EQ(n, 0);
}

TEST_F(AttributeMapTest, InitialiseAttributes)
{
  // Test state of constructed attr_map_2
  EXPECT_EQ(attr_map_2.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(attr_map_2.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_2.HasAttribute(VARIABLE_ATTRIBUTE));
  EXPECT_EQ(attr_map_2.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  std::string var_value = std::string("$") + DESCRIPTION_ATTRIBUTE;
  EXPECT_EQ(attr_map_2.GetAttribute(VARIABLE_ATTRIBUTE), var_value);

  // Test initialise with empty map
  EXPECT_FALSE(attr_map_2.InitialiseVariableAttributes(attr_map_0));
  EXPECT_EQ(attr_map_2.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(attr_map_2.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_2.HasAttribute(VARIABLE_ATTRIBUTE));
  EXPECT_EQ(attr_map_2.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_2.GetAttribute(VARIABLE_ATTRIBUTE), var_value);

  // Test initialise with non-empty map that lacks the variable definition
  EXPECT_TRUE(attr_map_0.AddAttribute(attributes::NAME_ATTRIBUTE, NAME_VALUE));

  EXPECT_FALSE(attr_map_2.InitialiseVariableAttributes(attr_map_0));
  EXPECT_EQ(attr_map_2.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(attr_map_2.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_2.HasAttribute(VARIABLE_ATTRIBUTE));
  EXPECT_EQ(attr_map_2.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_2.GetAttribute(VARIABLE_ATTRIBUTE), var_value);

  // Test initialise with map that contains the variable definition
  EXPECT_TRUE(attr_map_2.InitialiseVariableAttributes(attr_map_1));
  EXPECT_EQ(attr_map_2.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(attr_map_2.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(attr_map_2.HasAttribute(VARIABLE_ATTRIBUTE));
  EXPECT_EQ(attr_map_2.GetAttribute(attributes::NAME_ATTRIBUTE), NAME_VALUE);
  EXPECT_EQ(attr_map_2.GetAttribute(VARIABLE_ATTRIBUTE), DESCRIPTION_VALUE);
}

AttributeMapTest::AttributeMapTest()
{
  attr_map_1.AddAttribute(attributes::NAME_ATTRIBUTE, NAME_VALUE);
  attr_map_1.AddAttribute(DESCRIPTION_ATTRIBUTE, DESCRIPTION_VALUE);

  attr_map_2.AddAttribute(attributes::NAME_ATTRIBUTE, NAME_VALUE);
  std::string var_value = std::string("$") + DESCRIPTION_ATTRIBUTE;
  attr_map_2.AddAttribute(VARIABLE_ATTRIBUTE, var_value);
}

AttributeMapTest::~AttributeMapTest() = default;
