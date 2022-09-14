/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
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

#include <sup/sequencer/parser/treedata_xml_write_utils.h>

#include "unit_test_helper.h"

#include <sup/sequencer/tree_data.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class TreeDataXmlWriteUilsTest : public ::testing::Test
{
};

//! Write a single element.
TEST_F(TreeDataXmlWriteUilsTest, WriteElement)
{
  TreeData tree_data("Element");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element/>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteElement.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}

//! Write a single element with attribute.
TEST_F(TreeDataXmlWriteUilsTest, WriteElementWithAttribute)
{
  TreeData tree_data("Element");
  tree_data.AddAttribute("name", "James");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element name="James"/>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteElementWithAttribute.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}

//! Write a single element with attribute.
TEST_F(TreeDataXmlWriteUilsTest, WriteElementWithTwoAttributes)
{
  TreeData tree_data("Element");
  tree_data.AddAttribute("age", "33");
  tree_data.AddAttribute("name", "James");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element age="33" name="James"/>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteElementWithTwoAttributes.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}

//! Write a single element with content defined.
TEST_F(TreeDataXmlWriteUilsTest, WriteElementContent)
{
  TreeData tree_data("Element");
  tree_data.SetContent("1.0, 2.0, 3.0");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element>1.0, 2.0, 3.0</Element>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteElementContent.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}

//! Write a single element with attribute and content defined.
TEST_F(TreeDataXmlWriteUilsTest, WriteElementAttributeAndContent)
{
  TreeData tree_data("Element");
  tree_data.AddAttribute("name", "James");
  tree_data.SetContent("1.0, 2.0, 3.0");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element name="James">1.0, 2.0, 3.0</Element>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteElementAttributeAndContent.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}

//! Write a single element with attribute and content defined.
TEST_F(TreeDataXmlWriteUilsTest, WriteElementWithChildren)
{
  TreeData tree_data("Element");
  tree_data.AddChild(TreeData("Child"));
  tree_data.AddChild(TreeData("Child"));
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element>
  <Child/>
  <Child/>
</Element>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteElementWithChildren.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}

//! Write a single element with attribute and content defined.
TEST_F(TreeDataXmlWriteUilsTest, WriteItemData)
{
  TreeData tree_data("ItemData");
  TreeData variant1("Variant");
  variant1.AddAttribute("type", "int");
  variant1.AddAttribute("role", "0");
  variant1.SetContent("42");
  TreeData variant2("Variant");
  variant2.AddAttribute("type", "vector_double_t");
  variant2.AddAttribute("role", "0");
  variant2.SetContent("42.1, 42.2, 42.3");
  tree_data.AddChild(variant1);
  tree_data.AddChild(variant2);

  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<ItemData>
  <Variant role="0" type="int">42</Variant>
  <Variant role="0" type="vector_double_t">42.1, 42.2, 42.3</Variant>
</ItemData>
)"};

  // writing to string and checking content
  auto str = ::sup::sequencer::GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const std::string file_name = "WriteItemData.xml";
  WriteToXMLFile(file_name, tree_data);
  EXPECT_EQ(::sup::UnitTestHelper::GetTextFileContent(file_name), expected);
  std::remove(file_name.c_str());
}
