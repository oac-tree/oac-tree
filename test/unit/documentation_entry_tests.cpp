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
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/documentation_entry.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;
using namespace sup::oac_tree::Constants;

const auto kLongDescription =
R"RAW(This instruction does all kinds of things:
  * it can do A if you want
  * or else it does B
  * or maybe even something else
)RAW";

class DocumentationEntryTest : public ::testing::Test
{
protected:
  DocumentationEntryTest() = default;
  virtual ~DocumentationEntryTest() = default;
};

TEST_F(DocumentationEntryTest, DefaultCtor)
{
  // Default constructed DocumentationEntry does not contain any fields
  DocumentationEntry doc{};
  const std::string empty{};
  const std::string unknown{"Unknown"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), empty);
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION, unknown), unknown);
  const auto field_names = doc.GetFieldNames();
  EXPECT_TRUE(field_names.empty());
}

TEST_F(DocumentationEntryTest, MapCtor)
{
  // DocumentationEntry constructed from map
  const std::string description{"This instruction is well-documented!"};
  DocumentationEntry doc{{
    { INSTR_DOC_DESCRIPTION, description }
  }};
  const std::string empty{};
  const std::string unknown{"Unknown"};
  const std::string unknown_field_name{"unknown_field"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), description);
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION, unknown), description);
  EXPECT_EQ(doc.GetField(unknown_field_name), empty);
  EXPECT_EQ(doc.GetField(unknown_field_name, unknown), unknown);
  const auto field_names = doc.GetFieldNames();
  ASSERT_EQ(field_names.size(), 1);
  EXPECT_EQ(field_names[0], INSTR_DOC_DESCRIPTION);
}

TEST_F(DocumentationEntryTest, LongDescription)
{
  // DocumentationEntry constructed from map with multiline description
  DocumentationEntry doc{{
    { INSTR_DOC_DESCRIPTION, kLongDescription }
  }};
  const std::string empty{};
  const std::string unknown{"Unknown"};
  const std::string unknown_field_name{"unknown_field"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), kLongDescription);
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION, unknown), kLongDescription);
  EXPECT_EQ(doc.GetField(unknown_field_name), empty);
  EXPECT_EQ(doc.GetField(unknown_field_name, unknown), unknown);
  const auto field_names = doc.GetFieldNames();
  ASSERT_EQ(field_names.size(), 1);
  EXPECT_EQ(field_names[0], INSTR_DOC_DESCRIPTION);
}

TEST_F(DocumentationEntryTest, SetField)
{
  // DocumentationEntry constructed from map
  const std::string description{"This instruction is well-documented!"};
  DocumentationEntry doc{{
    { INSTR_DOC_DESCRIPTION, description }
  }};
  auto field_names = doc.GetFieldNames();
  ASSERT_EQ(field_names.size(), 1);
  const std::string empty{};
  const std::string field_name{"NewField"};
  const std::string field_value{"NewFieldValue"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), description);
  EXPECT_EQ(doc.GetField(field_name), empty);
  // Set field
  doc.SetField(field_name, field_value);
  EXPECT_EQ(doc.GetField(field_name), field_value);
  field_names = doc.GetFieldNames();
  ASSERT_EQ(field_names.size(), 2);
}
