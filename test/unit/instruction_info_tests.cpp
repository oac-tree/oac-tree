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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/sequencer/info_constants.h>
#include <sup/sequencer/instruction_info_utils.h>
#include <sup/sequencer/instruction_map.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <set>

using namespace sup::sequencer;

class InstructionInfoUtilsTest : public ::testing::Test
{
protected:
  InstructionInfoUtilsTest() = default;
  virtual ~InstructionInfoUtilsTest() = default;
};

TEST_F(InstructionInfoUtilsTest, InstructionInfoFromInstructionTree)
{
  // Construct procedure and extract Workspace
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_NO_THROW(proc->Setup());
  const auto* root = proc->RootInstruction();

  // Create InstructionInfo tree
  InstructionMap instr_map{root};
  auto instr_info = utils::CreateInstructionInfoTree(*root, instr_map);

  // Check root of the InstructionInfo tree
  EXPECT_EQ(instr_info->GetType(), "Sequence");
  std::set<sup::dto::uint32> indices;
  indices.insert(instr_info->GetIndex());
  auto attributes = instr_info->GetAttributes();
  EXPECT_EQ(attributes.size(), 0);

  auto children = instr_info->Children();
  ASSERT_EQ(children.size(), 2);
  {
    // check first child
    auto child = children[0];
    EXPECT_EQ(child->GetType(), "Copy");
    indices.insert(child->GetIndex());
    auto child_attrs = child->GetAttributes();
    EXPECT_EQ(child_attrs.size(), 2);
    EXPECT_EQ(child->Children().size(), 0);
  }
  {
    // check second child
    auto child = children[1];
    EXPECT_EQ(child->GetType(), "Copy");
    indices.insert(child->GetIndex());
    auto child_attrs = child->GetAttributes();
    EXPECT_EQ(child_attrs.size(), 2);
    EXPECT_EQ(child->Children().size(), 0);
  }
  std::set<sup::dto::uint32> expected_indices{0, 1, 2};
  EXPECT_EQ(indices, expected_indices);

  // Test copy
  InstructionInfo copy{*instr_info};
  EXPECT_EQ(copy, *instr_info);

  // Test move
  InstructionInfo moved{std::move(copy)};
  EXPECT_EQ(moved, *instr_info);
}

TEST_F(InstructionInfoUtilsTest, CreateOrderedInstructionInfo)
{
  InstructionInfo sequence("sequence", 0, {});
  auto child0 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child0", 1, {}));
  auto child0_ptr = child0.get();
  auto child1 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child1", 2, {}));
  auto child1_ptr = child1.get();

  sequence.AppendChild(std::move(child0));
  sequence.AppendChild(std::move(child1));

  auto ordered_info = utils::CreateOrderedInstructionInfo(sequence);
  ASSERT_EQ(ordered_info.size(), 3);

  EXPECT_EQ(ordered_info[0], &sequence);
  EXPECT_EQ(ordered_info[1], child0_ptr);
  EXPECT_EQ(ordered_info[2], child1_ptr);
}

TEST_F(InstructionInfoUtilsTest, InstructionInfoToFromAnyValue)
{
  // Construct procedure and extract Workspace
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_NO_THROW(proc->Setup());
  const auto* root = proc->RootInstruction();

  // Create InstructionInfo tree
  InstructionMap instr_map{root};
  auto instr_info = utils::CreateInstructionInfoTree(*root, instr_map);

  // Create AnyValue, translate back and check they are equal
  auto instr_av = utils::ToAnyValueTree(*instr_info);
  auto instr_info_read_back = utils::ToInstructionInfoTree(instr_av);
  EXPECT_EQ(*instr_info_read_back, *instr_info);
}
