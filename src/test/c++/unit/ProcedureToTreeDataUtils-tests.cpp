/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
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

#include "Instruction.h"
#include "LocalVariable.h"
#include "Procedure.h"
#include "ProcedureToTreeDataUtils.h"
#include "TreeData.h"
#include "Variable.h"

#include <gtest/gtest.h>

using namespace sup::sequencer;

class ProcedureToTreeDataUtilsTest : public ::testing::Test
{
};

TEST_F(ProcedureToTreeDataUtilsTest, VariableToTreeData)
{
  const std::string type_key("type");
  const std::string value_key("value");
  const std::string expected_type(R"RAW({"type":"uint32"})RAW");
  const std::string expected_value("42");

  LocalVariable variable;
  variable.AddAttribute(type_key, expected_type);
  variable.AddAttribute(value_key, expected_value);

  auto tree_data = ToTreeData(variable);

  EXPECT_EQ(tree_data->GetType(), variable.GetType());
  EXPECT_TRUE(tree_data->GetContent().empty());
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 0);
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 2);
  EXPECT_EQ(tree_data->GetAttribute(type_key), expected_type);
  EXPECT_EQ(tree_data->GetAttribute(value_key), expected_value);
}
