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

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_registry.h>
#include <sup/oac-tree/variable_utils.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;
using namespace sup::oac_tree::Constants;

class VariableInfoTest : public ::testing::Test
{
protected:
  VariableInfoTest() = default;
  virtual ~VariableInfoTest() = default;
};

TEST_F(VariableInfoTest, FromVariable)
{
  const std::string var_type = "Local";
  const sup::dto::uint32 var_idx = 42u;
  auto var = GlobalVariableRegistry().Create(var_type);

  // Check VariableInfo from Variable without attributes
  auto var_info = utils::CreateVariableInfo(var.get(), var_idx);
  EXPECT_EQ(var_info.GetType(), var_type);
  EXPECT_EQ(var_info.GetIndex(), var_idx);
  auto attributes = var_info.GetAttributes();
  EXPECT_EQ(attributes.size(), 0);

  // Check VariableInfo after adding attributes
  AttributeInfo attr_0{"one", "hello"};
  AttributeInfo attr_1{"two", "world"};
  var->AddAttribute(attr_0.first, attr_0.second);
  var->AddAttribute(attr_1.first, attr_1.second);
  var_info = utils::CreateVariableInfo(var.get(), var_idx);
  EXPECT_EQ(var_info.GetType(), var_type);
  EXPECT_EQ(var_info.GetIndex(), var_idx);
  attributes = var_info.GetAttributes();
  ASSERT_EQ(attributes.size(), 2);
  EXPECT_EQ(attributes[0], attr_0);
  EXPECT_EQ(attributes[1], attr_1);

  // Check copy
  VariableInfo copy{var_info};
  EXPECT_EQ(copy, var_info);

  // Check move
  VariableInfo moved{std::move(copy)};
  EXPECT_EQ(moved, var_info);
}

TEST_F(VariableInfoTest, FromNullptrVariable)
{
  Variable* var = nullptr;
  EXPECT_THROW(utils::CreateVariableInfo(var, 0), InvalidOperationException);
}

TEST_F(VariableInfoTest, ToFromAnyValue)
{
  {
    // Variable without attributes
    std::vector<AttributeInfo> attributes;
    VariableInfo var_info{"MyVarType", 33u, attributes};
    auto var_info_av = utils::ToAnyValue(var_info);
    auto var_info_read_back = utils::ToVariableInfo(var_info_av);
    EXPECT_EQ(var_info_read_back, var_info);
  }
  {
    // Variable with two attributes
    AttributeInfo attr_0{"one", "hello"};
    AttributeInfo attr_1{"two", "world"};
    std::vector<AttributeInfo> attributes;
    attributes.push_back(attr_0);
    attributes.push_back(attr_1);
    VariableInfo var_info{"MyVarType", 0, attributes};
    auto var_info_av = utils::ToAnyValue(var_info);
    auto var_info_read_back = utils::ToVariableInfo(var_info_av);
    EXPECT_EQ(var_info_read_back, var_info);
  }
}

TEST_F(VariableInfoTest, FromAnyValue)
{
  std::vector<AttributeInfo> expected_attrs{
      { "field_1", "val_1" },
      { "field_2", "val_2" },
      { "field_3", "val_3" }
  };
  {
    // Without attributes
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42u }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    ASSERT_NO_THROW(utils::ToVariableInfo(info_av));
    auto var_info = utils::ToVariableInfo(info_av);
    EXPECT_EQ(var_info.GetType(), "VarType");
    EXPECT_EQ(var_info.GetIndex(), 42u);
    auto attrs = var_info.GetAttributes();
    EXPECT_EQ(attrs.size(), 0);
  }
  {
    // Passing translation with attributes
    sup::dto::AnyValue attrs_av = {{
      { "field_1", "val_1" },
      { "field_2", "val_2" },
      { "field_3", "val_3" }
    }};
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42u }},
      { kAttributesField, attrs_av }
    }};
    ASSERT_NO_THROW(utils::ToVariableInfo(info_av));
    auto var_info = utils::ToVariableInfo(info_av);
    EXPECT_EQ(var_info.GetType(), "VarType");
    EXPECT_EQ(var_info.GetIndex(), 42u);
    auto attrs = var_info.GetAttributes();
    EXPECT_EQ(attrs.size(), 3);
    EXPECT_EQ(attrs, expected_attrs);
  }
  {
    // Passing translation with extra fields
    sup::dto::AnyValue attrs_av = {{
      { "field_1", "val_1" },
      { "field_2", "val_2" },
      { "field_3", "val_3" }
    }};
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42u }},
      { kAttributesField, attrs_av },
      { "ignored_field", true }
    }};
    ASSERT_NO_THROW(utils::ToVariableInfo(info_av));
    auto var_info = utils::ToVariableInfo(info_av);
    EXPECT_EQ(var_info.GetType(), "VarType");
    EXPECT_EQ(var_info.GetIndex(), 42u);
    auto attrs = var_info.GetAttributes();
    EXPECT_EQ(attrs.size(), 3);
    EXPECT_EQ(attrs, expected_attrs);
  }
  {
    // Missing type field
    sup::dto::AnyValue info_av = {{
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Empty type field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Wrong type for type field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, { sup::dto::BooleanType, true } },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Missing index field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Wrong type of index field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::SignedInteger16Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Missing attributes field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Attributes field not a structure
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, true }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
  {
    // Attributes field contains non-string members
    sup::dto::AnyValue attrs = {{
      { "field_1", "val_1" },
      { "field_2", false },
      { "field_3", "val_3" }
    }};
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, attrs }
    }};
    EXPECT_THROW(utils::ToVariableInfo(info_av), InvalidOperationException);
  }
}

TEST_F(VariableInfoTest, ValidateVariableInfoAnyValue)
{
  {
    // Passing validation without attributes
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_TRUE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Passing validation with attributes
    sup::dto::AnyValue attrs = {{
      { "field_1", "val_1" },
      { "field_2", "val_2" },
      { "field_3", "val_3" }
    }};
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, attrs }
    }};
    EXPECT_TRUE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Passing validation with extra fields
    sup::dto::AnyValue attrs = {{
      { "field_1", "val_1" },
      { "field_2", "val_2" },
      { "field_3", "val_3" }
    }};
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, attrs },
      { "ignored_field", true }
    }};
    EXPECT_TRUE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Missing type field
    sup::dto::AnyValue info_av = {{
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Empty type field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Wrong type for type field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, { sup::dto::BooleanType, true } },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Missing index field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Wrong type of index field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::SignedInteger16Type, 0 }},
      { kAttributesField, sup::dto::EmptyStruct() }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Missing attributes field
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Attributes field not a structure
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, true }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
  {
    // Attributes field contains non-string members
    sup::dto::AnyValue attrs = {{
      { "field_1", "val_1" },
      { "field_2", false },
      { "field_3", "val_3" }
    }};
    sup::dto::AnyValue info_av = {{
      { kVariableInfoTypeField, "VarType" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
      { kAttributesField, attrs }
    }};
    EXPECT_FALSE(utils::ValidateVariableInfoAnyValue(info_av));
  }
}
