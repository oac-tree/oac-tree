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

#include <app/oac-tree-cli/cl_utils.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class CLUtilsTest : public ::testing::Test
{
protected:
  CLUtilsTest() {};
  virtual ~CLUtilsTest() = default;
};

TEST_F(CLUtilsTest, UpdateInstructionStatus)
{
  EXPECT_EQ(GetLoadedPluginMessage(), kNoPluginsLoaded);
}
