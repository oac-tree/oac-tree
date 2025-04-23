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

#include <sup/oac-tree/anyvalue_utils.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class AnyValueUtilsTest : public ::testing::Test
{
protected:
  AnyValueUtilsTest() = default;
  virtual ~AnyValueUtilsTest() = default;
};

TEST_F(AnyValueUtilsTest, ValidateMemberType)
{
  const std::string mem_field_name = "TestMemberFieldName";
  sup::dto::AnyValue test_av = {{
    { mem_field_name, { sup::dto::UnsignedInteger32Type, 42u }}
  }};
  {
    // Validation passes when field is present with the correct type:
    EXPECT_TRUE(utils::ValidateMemberType(test_av, mem_field_name,
                                          sup::dto::UnsignedInteger32Type));
  }
  {
    // Validation fails when field is not present:
    EXPECT_FALSE(utils::ValidateMemberType(test_av, "WrongFieldName",
                                          sup::dto::UnsignedInteger32Type));
  }
  {
    // Validation fails when field has wrong type:
    EXPECT_FALSE(utils::ValidateMemberType(test_av, mem_field_name,
                                          sup::dto::SignedInteger16Type));
  }
  {
    // Validation fails when input AnyValue is not a struct:
    sup::dto::AnyValue scalar_av{ sup::dto::UnsignedInteger32Type, 42u };
    EXPECT_FALSE(utils::ValidateMemberType(scalar_av, mem_field_name,
                                          sup::dto::UnsignedInteger32Type));
  }
}
