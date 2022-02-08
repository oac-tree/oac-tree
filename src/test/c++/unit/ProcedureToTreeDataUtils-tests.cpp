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

#include "FileVariable.h"
#include "Instruction.h"
#include "InstructionRegistry.h"
#include "LocalVariable.h"
#include "Procedure.h"
#include "ProcedureToTreeDataUtils.h"
#include "Sequence.h"
#include "TreeData.h"
#include "Variable.h"
#include "VariableRegistry.h"
#include "Wait.h"
#include "Workspace.h"

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

TEST_F(ProcedureToTreeDataUtilsTest, EmptyWorkspaceToTreeData)
{
  Workspace workspace;

  auto tree_data = ToTreeData(workspace);
  EXPECT_EQ(tree_data->GetType(), std::string("Workspace"));  // FIXME replace with constant
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 0);
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_TRUE(tree_data->GetContent().empty());
}

TEST_F(ProcedureToTreeDataUtilsTest, WorkspaceWithSingleVariableToTreeData)
{
  Workspace workspace;

  // workspace with single variable
  const std::string value_key("value");
  const std::string expected_value("42");
  auto variable0 = GlobalVariableRegistry().Create(LocalVariable::Type);
  variable0->AddAttribute(value_key, expected_value);

  workspace.AddVariable("var0", variable0.release());
  auto tree_data = ToTreeData(workspace);
  EXPECT_EQ(tree_data->GetType(), std::string("Workspace"));  // FIXME replace with constant
  ASSERT_EQ(tree_data->GetNumberOfChildren(), 1);
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_TRUE(tree_data->GetContent().empty());

  auto variable_data = tree_data->Children().at(0);
  EXPECT_EQ(variable_data.GetType(), LocalVariable::Type);
  EXPECT_EQ(variable_data.GetAttribute(value_key), expected_value);
}

TEST_F(ProcedureToTreeDataUtilsTest, EmptySequenceToTreeData)
{
  auto sequence = GlobalInstructionRegistry().Create(Sequence::Type);
  sequence->AddAttribute("name", "abc");

  auto tree_data = ToTreeData(*sequence);
  EXPECT_EQ(tree_data->GetType(), Sequence::Type);
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 0);
  ASSERT_EQ(tree_data->GetNumberOfAttributes(), 1);
  EXPECT_EQ(tree_data->GetAttribute("name"), "abc");
  EXPECT_TRUE(tree_data->GetContent().empty());
}
