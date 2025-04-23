/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
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

#include <sup/oac-tree/scope_guard.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class ScopeGuardTest : public ::testing::Test
{
protected:
  ScopeGuardTest() = default;
  virtual ~ScopeGuardTest() = default;
};

TEST_F(ScopeGuardTest, Construction)
{
  int index = 0;
  auto incr_index = [&index]() { ++index; };
  {
    ScopeGuard guard{};
    EXPECT_FALSE(guard.IsValid());
    EXPECT_EQ(index, 0);
  }
  {
    ScopeGuard guard{incr_index};
    EXPECT_TRUE(guard.IsValid());
    EXPECT_EQ(index, 0);
  }
  EXPECT_EQ(index, 1);
}

TEST_F(ScopeGuardTest, Move)
{
  int index = 0;
  auto incr_index = [&index]() { ++index; };
  {
    ScopeGuard guard{incr_index};
    EXPECT_TRUE(guard.IsValid());
    ScopeGuard moved{std::move(guard)};
    EXPECT_FALSE(guard.IsValid());
    EXPECT_TRUE(moved.IsValid());
    EXPECT_EQ(index, 0);
  }
  EXPECT_EQ(index, 1);
  {
    ScopeGuard guard{incr_index};
    EXPECT_TRUE(guard.IsValid());
    ScopeGuard moved;
    EXPECT_FALSE(moved.IsValid());
    moved = std::move(guard);
    EXPECT_TRUE(moved.IsValid());
    EXPECT_FALSE(guard.IsValid());
    EXPECT_EQ(index, 1);
  }
  EXPECT_EQ(index, 2);
}

TEST_F(ScopeGuardTest, Swap)
{
  int index = 0;
  auto incr_index = [&index]() { ++index; };
  {
    ScopeGuard guard{incr_index};
    EXPECT_TRUE(guard.IsValid());
    ScopeGuard moved;
    EXPECT_FALSE(moved.IsValid());
    moved.Swap(guard);
    EXPECT_TRUE(moved.IsValid());
    EXPECT_FALSE(guard.IsValid());
    EXPECT_EQ(index, 0);
  }
  EXPECT_EQ(index, 1);
}
