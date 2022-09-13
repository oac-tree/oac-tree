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

#include <sup/sequencer/log.h>
#include <sup/sequencer/TreeData.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

class TreeDataTest : public ::testing::Test
{
protected:
  TreeDataTest();
  virtual ~TreeDataTest();

  TreeData data;
  TreeData parent;
  TreeData child_0;
  TreeData child_1;
};

static const bool kLogToStdOut = (log::SetStdOut(), true);

static const std::string TREE_TYPE = "TestData";
static const std::string CHILD0_TYPE = "Child_0";
static const std::string CHILD1_TYPE = "Child_1";
static const std::string CHILD0_NAME = "First child";
static const std::string ATTR_NAME_1 = "First attribute";
static const std::string ATTR_NAME_2 = "Second attribute";
static const std::string ATTR_VALUE_1 = "First value";
static const std::string ATTR_VALUE_2 = "Second value";

TEST_F(TreeDataTest, Constructed)
{
  // Types
  EXPECT_EQ(data.GetType(), TREE_TYPE);
  EXPECT_EQ(child_0.GetType(), CHILD0_TYPE);
  EXPECT_EQ(child_1.GetType(), CHILD1_TYPE);

  // Name, attributes, child elements and content
  std::string empty_str;
  EXPECT_EQ(data.GetName(), empty_str);
  EXPECT_FALSE(data.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(data.GetAttribute(attributes::NAME_ATTRIBUTE), empty_str);
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  auto& attr_map = data.Attributes();
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  auto& children = data.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_EQ(data.GetContent(), empty_str);

  int n = 0;
  for (auto attr : data.Attributes())
  {
    ++n;
  }
  EXPECT_EQ(n, 0);
}

TEST_F(TreeDataTest, CopyConstructed)
{
  TreeData data_copy = parent;
  EXPECT_EQ(data_copy, parent);
  EXPECT_FALSE(data_copy != parent);
  EXPECT_EQ(data_copy.GetType(), parent.GetType());
  EXPECT_EQ(data_copy.GetContent(), parent.GetContent());
  EXPECT_EQ(data_copy.GetNumberOfAttributes(), parent.GetNumberOfAttributes());
  EXPECT_EQ(data_copy.GetNumberOfChildren(), parent.GetNumberOfChildren());

  for (auto attr : parent.Attributes())
  {
    EXPECT_TRUE(data_copy.HasAttribute(attr.first));
    EXPECT_EQ(data_copy.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(TreeDataTest, MoveConstructed)
{
  TreeData tmp_data = parent;  // make copy first to allow comparison later on
  TreeData data_target = std::move(tmp_data);
  EXPECT_EQ(data_target, parent);
  EXPECT_FALSE(data_target != parent);
  EXPECT_EQ(data_target.GetType(), parent.GetType());
  EXPECT_EQ(data_target.GetContent(), parent.GetContent());
  EXPECT_EQ(data_target.GetNumberOfAttributes(), parent.GetNumberOfAttributes());
  EXPECT_EQ(data_target.GetNumberOfChildren(), parent.GetNumberOfChildren());

  for (auto attr : parent.Attributes())
  {
    EXPECT_TRUE(data_target.HasAttribute(attr.first));
    EXPECT_EQ(data_target.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(TreeDataTest, CopyAssigned)
{
  data = parent;
  EXPECT_EQ(data, parent);
  EXPECT_FALSE(data != parent);
  EXPECT_EQ(data.GetType(), parent.GetType());
  EXPECT_EQ(data.GetContent(), parent.GetContent());
  EXPECT_EQ(data.GetNumberOfAttributes(), parent.GetNumberOfAttributes());
  EXPECT_EQ(data.GetNumberOfChildren(), parent.GetNumberOfChildren());

  for (auto attr : parent.Attributes())
  {
    EXPECT_TRUE(data.HasAttribute(attr.first));
    EXPECT_EQ(data.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(TreeDataTest, MoveAssigned)
{
  TreeData tmp_data = parent;  // make copy first to allow comparison later on
  data = std::move(tmp_data);
  EXPECT_EQ(data, parent);
  EXPECT_FALSE(data != parent);
  EXPECT_EQ(data.GetType(), parent.GetType());
  EXPECT_EQ(data.GetContent(), parent.GetContent());
  EXPECT_EQ(data.GetNumberOfAttributes(), parent.GetNumberOfAttributes());
  EXPECT_EQ(data.GetNumberOfChildren(), parent.GetNumberOfChildren());

  for (auto attr : parent.Attributes())
  {
    EXPECT_TRUE(data.HasAttribute(attr.first));
    EXPECT_EQ(data.GetAttribute(attr.first), attr.second);
  }
}

TEST_F(TreeDataTest, SetName)
{
  // Empty on construction
  std::string empty_str;
  EXPECT_EQ(data.GetName(), empty_str);
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);

  // Set original name
  std::string first_name = "Original name";
  data.SetName(first_name);
  EXPECT_EQ(data.GetName(), first_name);
  EXPECT_TRUE(data.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(data.GetAttribute(attributes::NAME_ATTRIBUTE), first_name);
  EXPECT_EQ(data.GetNumberOfAttributes(), 1);
  auto attr_map = data.Attributes();
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), 1);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  auto children = data.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_EQ(data.GetContent(), empty_str);

  // Overwrite the name
  std::string second_name = "Overwritten name";
  data.SetName(second_name);
  EXPECT_EQ(data.GetName(), second_name);
  EXPECT_TRUE(data.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(data.GetAttribute(attributes::NAME_ATTRIBUTE), second_name);
  EXPECT_EQ(data.GetNumberOfAttributes(), 1);
  attr_map = data.Attributes();
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), 1);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  children = data.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_EQ(data.GetContent(), empty_str);
}

TEST_F(TreeDataTest, AddAttribute)
{
  // Add a first attribute
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  EXPECT_FALSE(data.HasAttribute(ATTR_NAME_1));
  EXPECT_FALSE(data.HasAttribute(ATTR_NAME_2));
  EXPECT_TRUE(data.AddAttribute(ATTR_NAME_1, ATTR_VALUE_1));
  EXPECT_EQ(data.GetNumberOfAttributes(), 1);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  EXPECT_TRUE(data.HasAttribute(ATTR_NAME_1));
  EXPECT_FALSE(data.HasAttribute(ATTR_NAME_2));
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_1), ATTR_VALUE_1);
  std::string empty_str;
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_2), empty_str);
  int n = 0;
  for (auto attr : data.Attributes())
  {
    EXPECT_EQ(attr.first, ATTR_NAME_1);
    EXPECT_EQ(attr.second, ATTR_VALUE_1);
    ++n;
  }
  EXPECT_EQ(n, 1);

  // Add a second attribute
  EXPECT_TRUE(data.AddAttribute(ATTR_NAME_2, ATTR_VALUE_2));
  EXPECT_EQ(data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  EXPECT_TRUE(data.HasAttribute(ATTR_NAME_1));
  EXPECT_TRUE(data.HasAttribute(ATTR_NAME_2));
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_1), ATTR_VALUE_1);
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_2), ATTR_VALUE_2);
  n = 0;
  for (auto attr : data.Attributes())
  {
    ++n;
  }
  EXPECT_EQ(n, 2);

  // Try to overwrite the first attribute by using AddAttribute (should fail)
  std::string new_value = "New value";
  EXPECT_FALSE(data.AddAttribute(ATTR_NAME_1, new_value));
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_1), ATTR_VALUE_1);
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_2), ATTR_VALUE_2);
  EXPECT_EQ(data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
}

TEST_F(TreeDataTest, SetAttribute)
{
  // Set a first attribute
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  EXPECT_FALSE(data.HasAttribute(ATTR_NAME_1));
  EXPECT_FALSE(data.HasAttribute(ATTR_NAME_2));
  data.AddAttribute(ATTR_NAME_1, ATTR_VALUE_1);
  EXPECT_EQ(data.GetNumberOfAttributes(), 1);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  EXPECT_TRUE(data.HasAttribute(ATTR_NAME_1));
  EXPECT_FALSE(data.HasAttribute(ATTR_NAME_2));
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_1), ATTR_VALUE_1);
  std::string empty_str;
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_2), empty_str);

  // Set a second attribute
  data.SetAttribute(ATTR_NAME_2, ATTR_VALUE_2);
  EXPECT_EQ(data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  EXPECT_TRUE(data.HasAttribute(ATTR_NAME_1));
  EXPECT_TRUE(data.HasAttribute(ATTR_NAME_2));
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_1), ATTR_VALUE_1);
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_2), ATTR_VALUE_2);

  // Try to overwrite the first attribute by using SetAttribute (should succeed)
  std::string new_value = "Overwritten value";
  data.SetAttribute(ATTR_NAME_1, new_value);
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_1), new_value);
  EXPECT_EQ(data.GetAttribute(ATTR_NAME_2), ATTR_VALUE_2);
  EXPECT_EQ(data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
}

TEST_F(TreeDataTest, RemoveAttribute)
{
  EXPECT_EQ(child_0.GetNumberOfAttributes(), 2);
  EXPECT_EQ(child_0.GetNumberOfChildren(), 0);
  EXPECT_TRUE(child_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_TRUE(child_0.HasAttribute(ATTR_NAME_1));

  std::string does_not_exist = "Nonexistent attribute name";
  EXPECT_FALSE(child_0.RemoveAttribute(does_not_exist));
  EXPECT_EQ(child_0.GetNumberOfAttributes(), 2);
  EXPECT_EQ(child_0.GetNumberOfChildren(), 0);

  EXPECT_TRUE(child_0.RemoveAttribute(ATTR_NAME_1));
  EXPECT_EQ(child_0.GetNumberOfAttributes(), 1);
  EXPECT_EQ(child_0.GetNumberOfChildren(), 0);
  EXPECT_FALSE(child_0.HasAttribute(ATTR_NAME_1));
  EXPECT_TRUE(child_0.HasAttribute(attributes::NAME_ATTRIBUTE));
  EXPECT_EQ(child_0.GetAttribute(attributes::NAME_ATTRIBUTE), CHILD0_NAME);
}

TEST_F(TreeDataTest, AddChild)
{
  // Add a first child
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 0);
  data.AddChild(child_0);
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 1);
  auto children = data.Children();
  EXPECT_EQ(children.size(), 1);
  int n = 0;
  for (auto child : children)
  {
    EXPECT_EQ(child.GetNumberOfAttributes(), 2);
    EXPECT_EQ(child.GetNumberOfChildren(), 0);
    EXPECT_TRUE(child.HasAttribute(attributes::NAME_ATTRIBUTE));
    EXPECT_TRUE(child.HasAttribute(ATTR_NAME_1));
    EXPECT_EQ(child.GetAttribute(attributes::NAME_ATTRIBUTE), CHILD0_NAME);
    EXPECT_EQ(child.GetAttribute(ATTR_NAME_1), ATTR_VALUE_1);
    ++n;
  }
  EXPECT_EQ(n, 1);

  // Add a second child
  data.AddChild(child_1);
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(data.GetNumberOfChildren(), 2);
  children = data.Children();
  EXPECT_EQ(children.size(), 2);
  n = 0;
  for (auto child : children)
  {
    ++n;
  }
  EXPECT_EQ(n, 2);
}

TEST_F(TreeDataTest, SetContent)
{
  // Empty on construction
  std::string empty_str;
  EXPECT_EQ(data.GetContent(), empty_str);

  // Set original content
  std::string first_content = "Original content";
  data.SetContent(first_content);
  EXPECT_EQ(data.GetContent(), first_content);
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  auto attr_map = data.Attributes();
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), 0);
  auto children = data.Children();
  EXPECT_EQ(children.size(), 0);

  // Overwrite the content (should succeed)
  std::string second_content = "Overwritten content";
  data.SetContent(second_content);
  EXPECT_EQ(data.GetContent(), second_content);
  EXPECT_EQ(data.GetNumberOfAttributes(), 0);
  attr_map = data.Attributes();
  EXPECT_EQ(attr_map.GetNumberOfAttributes(), 0);
  children = data.Children();
  EXPECT_EQ(children.size(), 0);
}

TreeDataTest::TreeDataTest()
    : data{TREE_TYPE}, parent{TREE_TYPE}, child_0{CHILD0_TYPE}, child_1{CHILD1_TYPE}
{
  child_0.SetName(CHILD0_NAME);
  child_0.AddAttribute(ATTR_NAME_1, ATTR_VALUE_1);
  parent.AddChild(child_0);
  parent.AddChild(child_1);
}

TreeDataTest::~TreeDataTest() = default;
