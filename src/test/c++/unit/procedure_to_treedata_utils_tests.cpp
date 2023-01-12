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

#include <sup/sequencer/parser/procedure_to_treedata_utils.h>

#include <sup/sequencer/variables/file_variable.h>
#include <sup/sequencer/variables/local_variable.h>
#include <sup/sequencer/instructions/sequence.h>
#include <sup/sequencer/instructions/wait.h>

#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>
#include <sup/sequencer/workspace.h>

#include <sup/xml/tree_data.h>

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

  EXPECT_EQ(tree_data->GetNodeName(), variable.GetType());
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
  EXPECT_EQ(tree_data->GetNodeName(), Constants::WORKSPACE_ELEMENT_NAME);
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 0);
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_TRUE(tree_data->GetContent().empty());
}

TEST_F(ProcedureToTreeDataUtilsTest, WorkspaceWithSingleVariableToTreeData)
{
  Workspace workspace;

  // workspace with single variable
  const std::string variable_attr_name("value");
  const std::string variable_attr_value("42");
  auto variable0 = GlobalVariableRegistry().Create(LocalVariable::Type);
  variable0->AddAttribute(variable_attr_name, variable_attr_value);

  workspace.AddVariable("var0", variable0.release());
  auto tree_data = ToTreeData(workspace);
  EXPECT_EQ(tree_data->GetNodeName(), Constants::WORKSPACE_ELEMENT_NAME);
  ASSERT_EQ(tree_data->GetNumberOfChildren(), 1);
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_TRUE(tree_data->GetContent().empty());

  auto variable_data = tree_data->Children().at(0);
  EXPECT_EQ(variable_data.GetNodeName(), LocalVariable::Type);
  EXPECT_EQ(variable_data.GetAttribute(variable_attr_name), variable_attr_value);
}

TEST_F(ProcedureToTreeDataUtilsTest, EmptySequenceToTreeData)
{
  const std::string instruction_attr_name("name");
  const std::string instruction_attr_value("abc");

  auto sequence = GlobalInstructionRegistry().Create(Sequence::Type);
  sequence->AddAttribute(instruction_attr_name, instruction_attr_value);

  auto tree_data = ToTreeData(*sequence);
  EXPECT_EQ(tree_data->GetNodeName(), Sequence::Type);
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 0);
  ASSERT_EQ(tree_data->GetNumberOfAttributes(), 1);
  EXPECT_EQ(tree_data->GetAttribute(instruction_attr_name), instruction_attr_value);
  EXPECT_TRUE(tree_data->GetContent().empty());
}

TEST_F(ProcedureToTreeDataUtilsTest, SequenceWithTwoChildrenToTreeData)
{
  auto sequence = GlobalInstructionRegistry().Create(Sequence::Type);

  const std::string wait0_attr_name("timeout");
  const std::string wait0_attr_value("42");
  auto wait0 = GlobalInstructionRegistry().Create(Wait::Type);
  wait0->AddAttribute(wait0_attr_name, wait0_attr_value);

  const std::string wait1_attr_name("timeout");
  const std::string wait1_attr_value("43");
  auto wait1 = GlobalInstructionRegistry().Create(Wait::Type);
  wait1->AddAttribute(wait1_attr_name, wait1_attr_value);

  sequence->InsertInstruction(wait0.release(), 0);
  sequence->InsertInstruction(wait1.release(), 1);

  auto tree_data = ToTreeData(*sequence);
  EXPECT_EQ(tree_data->GetNodeName(), Sequence::Type);
  ASSERT_EQ(tree_data->GetNumberOfChildren(), 2);
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_TRUE(tree_data->GetContent().empty());

  auto wait0_data = tree_data->Children().at(0);
  EXPECT_EQ(wait0_data.GetNodeName(), Wait::Type);
  EXPECT_EQ(wait0_data.GetNumberOfChildren(), 0);
  ASSERT_EQ(wait0_data.GetNumberOfAttributes(), 1);
  EXPECT_EQ(wait0_data.GetAttribute(wait0_attr_name), wait0_attr_value);
  EXPECT_TRUE(wait0_data.GetContent().empty());

  auto wait1_data = tree_data->Children().at(1);
  EXPECT_EQ(wait1_data.GetNodeName(), Wait::Type);
  EXPECT_EQ(wait1_data.GetNumberOfChildren(), 0);
  ASSERT_EQ(wait1_data.GetNumberOfAttributes(), 1);
  EXPECT_EQ(wait1_data.GetAttribute(wait1_attr_name), wait1_attr_value);
  EXPECT_TRUE(wait1_data.GetContent().empty());
}

TEST_F(ProcedureToTreeDataUtilsTest, EmptyProcedureToTreeData)
{
  Procedure procedure;

  auto tree_data = ToTreeData(procedure);
  EXPECT_EQ(tree_data->GetNodeName(), Constants::PROCEDURE_ELEMENT_NAME);
  ASSERT_EQ(tree_data->GetNumberOfChildren(), 1);    // corresponds to workspace
  ASSERT_EQ(tree_data->GetNumberOfAttributes(), 0);  // no schema or other attributes
  EXPECT_TRUE(tree_data->GetContent().empty());

  auto workspace_data = tree_data->Children().at(0);
  EXPECT_EQ(workspace_data.GetNodeName(), Constants::WORKSPACE_ELEMENT_NAME);
  EXPECT_EQ(workspace_data.GetNumberOfChildren(), 0);
  EXPECT_EQ(workspace_data.GetNumberOfAttributes(), 0);
  EXPECT_TRUE(workspace_data.GetContent().empty());
}

TEST_F(ProcedureToTreeDataUtilsTest, ProcedureWithSingleInstructionAndSingleVariableToTreeData)
{
  Procedure procedure;

  auto wait = GlobalInstructionRegistry().Create(Wait::Type);
  const std::string instruction_attr_name("value");
  const std::string instruction_attr_value("42");
  wait->AddAttribute(instruction_attr_name, instruction_attr_value);
  procedure.PushInstruction(wait.release());

  const std::string variable_attr_name("timeout");
  const std::string variable_attr_value("43");
  auto variable = GlobalVariableRegistry().Create(LocalVariable::Type);
  variable->AddAttribute(variable_attr_name, variable_attr_value);
  procedure.AddVariable("var0", variable.release());

  auto tree_data = ToTreeData(procedure);

  EXPECT_EQ(tree_data->GetNodeName(), Constants::PROCEDURE_ELEMENT_NAME);
  ASSERT_EQ(tree_data->GetNumberOfChildren(), 2);    // single instruction, workspace
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);  // no schema or other attributes
  EXPECT_TRUE(tree_data->GetContent().empty());

  auto wait_data = tree_data->Children().at(0);
  EXPECT_EQ(wait_data.GetNodeName(), Wait::Type);
  EXPECT_EQ(wait_data.GetNumberOfChildren(), 0);
  EXPECT_EQ(wait_data.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(wait_data.GetContent().empty());
  EXPECT_EQ(wait_data.GetAttribute(instruction_attr_name), instruction_attr_value);

  auto workspace_data = tree_data->Children().at(1);
  EXPECT_EQ(workspace_data.GetNodeName(), Constants::WORKSPACE_ELEMENT_NAME);
  ASSERT_EQ(workspace_data.GetNumberOfChildren(), 1);
  EXPECT_EQ(workspace_data.GetNumberOfAttributes(), 0);
  EXPECT_TRUE(workspace_data.GetContent().empty());

  auto variable_data = workspace_data.Children().at(0);
  EXPECT_EQ(variable_data.GetNodeName(), LocalVariable::Type);
  EXPECT_EQ(variable_data.GetAttribute(variable_attr_name), variable_attr_value);
}

TEST_F(ProcedureToTreeDataUtilsTest, ProcedureWithSequenceToTreeData)
{
  Procedure procedure;

  auto wait = GlobalInstructionRegistry().Create(Wait::Type);
  const std::string instruction_attr_name("value");
  const std::string instruction_attr_value("42");
  wait->AddAttribute(instruction_attr_name, instruction_attr_value);

  auto sequence = GlobalInstructionRegistry().Create(Sequence::Type);
  sequence->InsertInstruction(wait.release(), 0);

  procedure.PushInstruction(sequence.release());

  auto tree_data = ToTreeData(procedure);

  EXPECT_EQ(tree_data->GetNodeName(), Constants::PROCEDURE_ELEMENT_NAME);
  ASSERT_EQ(tree_data->GetNumberOfChildren(), 2); // sequence, workspace
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_TRUE(tree_data->GetContent().empty());

  auto sequence_data = tree_data->Children().at(0);
  EXPECT_EQ(sequence_data.GetNodeName(), Sequence::Type);
  ASSERT_EQ(sequence_data.GetNumberOfChildren(), 1);

  auto wait_data = sequence_data.Children().at(0);
  EXPECT_EQ(wait_data.GetNodeName(), Wait::Type);
  ASSERT_EQ(wait_data.GetNumberOfChildren(), 0);
}
