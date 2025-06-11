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

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_filters.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

/**
 * This list of parent indices represents the following tree:
 * 0
 * ├── 3
 * │   ├── 2
 * │   └── 1
 * └── 4
 *
 * The indices are not in the normal order they would appear if they would come from a real
 * instruction tree, but are chosen to allow for more random conditions.
 */
const std::vector<sup::dto::uint32> test_parent_indices = { kIsRootIndex, 3, 3, 0, 0 };

class InstructionFiltersTest : public ::testing::Test
{
protected:
  InstructionFiltersTest();
  virtual ~InstructionFiltersTest();
};

TEST_F(InstructionFiltersTest, Construction)
{
  {
    // Valid list of parent indices
    EXPECT_NO_THROW(AncestorsActiveFilter{test_parent_indices});
  }
  {
    // Parent indices out of range
    std::vector<sup::dto::uint32> invalid_indices = { kIsRootIndex, 0, 1, 5, 2 };
    EXPECT_THROW(AncestorsActiveFilter{invalid_indices}, InvalidOperationException);
  }
  {
    // No root node
    std::vector<sup::dto::uint32> no_root_indices = { 0, 0, 1, 2 };
    EXPECT_THROW(AncestorsActiveFilter{no_root_indices}, InvalidOperationException);
  }
  {
    // Two root nodes
    std::vector<sup::dto::uint32> two_root_indices = { kIsRootIndex, kIsRootIndex, 0, 1 };
    EXPECT_THROW(AncestorsActiveFilter{two_root_indices}, InvalidOperationException);
  }
  {
    // Loop
    std::vector<sup::dto::uint32> loop_indices = { kIsRootIndex, 3, 1, 2 };
    EXPECT_THROW(AncestorsActiveFilter{loop_indices}, InvalidOperationException);
  }
}

TEST_F(InstructionFiltersTest, Filter)
{
  AncestorsActiveFilter filter{test_parent_indices};
  {
  // Test with an empty set
  std::set<sup::dto::uint32> empty_set{};
  EXPECT_EQ(filter(empty_set), empty_set);
  }
  {
  // Test with a set containing all nodes
  std::set<sup::dto::uint32> full_set = { 0, 1, 2, 3, 4 };
  EXPECT_EQ(filter(full_set), full_set);
  }
  {
  // Test with a set containing a node that has a non-active parent (1)
  std::set<sup::dto::uint32> partial_set = { 0, 1, 4 };
  std::set<sup::dto::uint32> expected_set = { 0, 4 };
  EXPECT_EQ(filter(partial_set), expected_set);
  }
  {
  // Test with a set missing the root node
  std::set<sup::dto::uint32> partial_set = { 1, 2, 3, 4 };
  std::set<sup::dto::uint32> empty_set{};
  EXPECT_EQ(filter(partial_set), empty_set);
  }
  {
  // Test with a set containing an unknown node (8)
  std::set<sup::dto::uint32> unknown_node_set = { 0, 3, 8 };
  std::set<sup::dto::uint32> empty_set{};
  EXPECT_EQ(filter(unknown_node_set), empty_set);
  }
}

InstructionFiltersTest::InstructionFiltersTest() = default;

InstructionFiltersTest::~InstructionFiltersTest() = default;
