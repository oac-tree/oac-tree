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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction_doc.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;
using namespace sup::oac_tree::Constants;

const auto kLongDescription =
R"RAW(This instruction does all kinds of things:
  * it can do A if you want
  * or else it does B
  * or maybe even something else
)RAW";

class InstructionDocTest : public ::testing::Test
{
protected:
  InstructionDocTest() = default;
  virtual ~InstructionDocTest() = default;
};

TEST_F(InstructionDocTest, DefaultCtor)
{
  // Default constructed InstructionDoc does not contain any fields
  InstructionDoc doc{};
  const std::string empty{};
  const std::string unknown{"Unknown"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), empty);
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION, unknown), unknown);
}

TEST_F(InstructionDocTest, MapCtor)
{
  // InstructionDoc constructed from map
  const std::string description{"This instruction is well-documented!"};
  InstructionDoc doc{{
    { INSTR_DOC_DESCRIPTION, description }
  }};
  const std::string empty{};
  const std::string unknown{"Unknown"};
  const std::string unknown_field_name{"unknown_field"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), description);
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION, unknown), description);
  EXPECT_EQ(doc.GetField(unknown_field_name), empty);
  EXPECT_EQ(doc.GetField(unknown_field_name, unknown), unknown);
}

TEST_F(InstructionDocTest, LongDescription)
{
  // InstructionDoc constructed from map with multiline description
  InstructionDoc doc{{
    { INSTR_DOC_DESCRIPTION, kLongDescription }
  }};
  const std::string empty{};
  const std::string unknown{"Unknown"};
  const std::string unknown_field_name{"unknown_field"};
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION), kLongDescription);
  EXPECT_EQ(doc.GetField(INSTR_DOC_DESCRIPTION, unknown), kLongDescription);
  EXPECT_EQ(doc.GetField(unknown_field_name), empty);
  EXPECT_EQ(doc.GetField(unknown_field_name, unknown), unknown);
}
