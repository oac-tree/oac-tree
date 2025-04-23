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
#include <sup/oac-tree/variable_map.h>
#include <sup/oac-tree/variable_registry.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class VariableMapTest : public ::testing::Test
{
protected:
  VariableMapTest();
  virtual ~VariableMapTest();
};

TEST_F(VariableMapTest, Construction)
{
  {
    // Empty workspace
    Workspace ws;
    VariableMap var_map{ws};
    EXPECT_EQ(var_map.GetNumberOfVariables(), 0);
    const auto& index_map = var_map.GetVariableIndexMap();
    EXPECT_TRUE(index_map.empty());
    EXPECT_THROW(var_map.FindVariableIndex("there_is_no_variable"), InvalidOperationException);
  }
  {
    // Workspace with single variable
    Workspace ws;
    ws.AddVariable("var1", GlobalVariableRegistry().Create("Local"));
    VariableMap var_map{ws};
    EXPECT_EQ(var_map.GetNumberOfVariables(), 1);
    const auto& index_map = var_map.GetVariableIndexMap();
    EXPECT_EQ(index_map.size(), 1);
    EXPECT_EQ(var_map.FindVariableIndex("var1"), 0);
    EXPECT_THROW(var_map.FindVariableIndex("unknown_variable"), InvalidOperationException);
  }
  {
    // Workspace with three variables
    Workspace ws;
    ws.AddVariable("var1", GlobalVariableRegistry().Create("Local"));
    ws.AddVariable("var2", GlobalVariableRegistry().Create("Local"));
    ws.AddVariable("var3", GlobalVariableRegistry().Create("Local"));
    VariableMap var_map{ws};
    EXPECT_EQ(var_map.GetNumberOfVariables(), 3);
    const auto& index_map = var_map.GetVariableIndexMap();
    EXPECT_EQ(index_map.size(), 3);
    EXPECT_EQ(var_map.FindVariableIndex("var1"), 0);
    EXPECT_EQ(var_map.FindVariableIndex("var2"), 1);
    EXPECT_EQ(var_map.FindVariableIndex("var3"), 2);
    EXPECT_THROW(var_map.FindVariableIndex("unknown_variable"), InvalidOperationException);
  }
}

VariableMapTest::VariableMapTest() = default;

VariableMapTest::~VariableMapTest() = default;
