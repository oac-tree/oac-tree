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

#include <sup/oac-tree/generic_utils.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

const std::string TEST_FILE_NAME = "GenericUtilsTestFile";

class GenericUtilsTest : public ::testing::Test
{
protected:
  GenericUtilsTest();
  virtual ~GenericUtilsTest();

  sup::UnitTestHelper::TemporaryTestFile m_test_file;
};

TEST_F(GenericUtilsTest, FileExists)
{
  EXPECT_TRUE(utils::FileExists(TEST_FILE_NAME));
  EXPECT_FALSE(utils::FileExists("DoesNotExist"));
}

TEST_F(GenericUtilsTest, GetEnvironmentVariable)
{
  auto env_var = utils::GetEnvironmentVariable("PATH");
  EXPECT_FALSE(env_var.empty());
  env_var = utils::GetEnvironmentVariable("THIS_ENVIRONMENT_VARIABLE_DOES_NOT_EXIST");
  EXPECT_TRUE(env_var.empty());
}

GenericUtilsTest::GenericUtilsTest()
    : m_test_file(TEST_FILE_NAME, "test")
{}

GenericUtilsTest::~GenericUtilsTest() = default;
