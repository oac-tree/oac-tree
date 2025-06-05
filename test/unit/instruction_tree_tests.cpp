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

#include <sup/oac-tree/instruction_tree.h>
#include <sup/oac-tree/procedure_context.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

const std::string kSequenceType = "Sequence";
const std::string kInverterType = "Inverter";
const std::string kWaitType = "Wait";


class InstructionTreeTest : public ::testing::Test
{
protected:
  InstructionTreeTest();
  virtual ~InstructionTreeTest();
};

std::unique_ptr<Instruction> CreateTestInstructionTree();

TEST_F(InstructionTreeTest, BasicMethods)
{
  auto instr_0 = reinterpret_cast<Instruction*>(0u);
  auto instr_1 = reinterpret_cast<Instruction*>(1u);
  auto instr_2 = reinterpret_cast<Instruction*>(2u);
  auto instr_3 = reinterpret_cast<Instruction*>(3u);
  // Test methods on empty tree
  InstructionTree empty_tree{instr_0};
  ASSERT_TRUE(empty_tree.IsEmpty());

  // Test methods on tree existing of one element
  InstructionTree root_tree{instr_1};
  ASSERT_FALSE(root_tree.IsEmpty());
  EXPECT_EQ(root_tree.GetInstruction(), instr_1);
  auto root_children = root_tree.GetChildren();
  EXPECT_EQ(root_children.size(), 0);
  auto root_instructions = root_tree.GetChildInstructions();
  EXPECT_EQ(root_instructions.size(), 0);

  // Test methods on tree with one element
  auto t_2 = root_tree.AddChildInstruction(instr_2);
  EXPECT_EQ(t_2->GetInstruction(), instr_2);
  root_children = root_tree.GetChildren();
  ASSERT_EQ(root_children.size(), 1);
  EXPECT_EQ(root_children[0], t_2);
  root_instructions = root_tree.GetChildInstructions();
  ASSERT_EQ(root_instructions.size(), 1);
  EXPECT_EQ(root_instructions[0], instr_2);

  // Test methods on tree with two elements
  auto t_3 = root_tree.AddChildInstruction(instr_3);
  EXPECT_EQ(t_3->GetInstruction(), instr_3);
  root_children = root_tree.GetChildren();
  ASSERT_EQ(root_children.size(), 2);
  EXPECT_EQ(root_children[0], t_2);
  EXPECT_EQ(root_children[1], t_3);
  root_instructions = root_tree.GetChildInstructions();
  ASSERT_EQ(root_instructions.size(), 2);
  EXPECT_EQ(root_instructions[0], instr_2);
  EXPECT_EQ(root_instructions[1], instr_3);
}

TEST_F(InstructionTreeTest, CreateFullInstructionTree)
{
  // Test creation of full instruction tree from empty instruction
  EXPECT_THROW(CreateFullInstructionTree(nullptr), InvalidOperationException);

  // Test root node of full instruction tree
  auto sequence = CreateTestInstructionTree();
  auto full_tree = CreateFullInstructionTree(sequence.get());
  ASSERT_FALSE(full_tree.IsEmpty());
  EXPECT_EQ(full_tree.GetInstruction(), sequence.get());
  auto children_seq_1 = full_tree.GetChildren();
  auto child_instr_seq_1 = full_tree.GetChildInstructions();
  auto seq_1_children = sequence->ChildInstructions();
  ASSERT_EQ(children_seq_1.size(), 3);
  ASSERT_EQ(child_instr_seq_1.size(), 3);
  ASSERT_EQ(seq_1_children.size(), 3);
  EXPECT_EQ(children_seq_1[0]->GetInstruction(), seq_1_children[0]);
  EXPECT_EQ(children_seq_1[1]->GetInstruction(), seq_1_children[1]);
  EXPECT_EQ(children_seq_1[2]->GetInstruction(), seq_1_children[2]);
  EXPECT_EQ(child_instr_seq_1[0], seq_1_children[0]);
  EXPECT_EQ(child_instr_seq_1[1], seq_1_children[1]);
  EXPECT_EQ(child_instr_seq_1[2], seq_1_children[2]);

  // Test embedded sequence node of instruction tree
  auto children_seq_2 = children_seq_1[1]->GetChildren();
  auto child_instr_seq_2 = children_seq_1[1]->GetChildInstructions();
  auto seq_2_children = seq_1_children[1]->ChildInstructions();
  ASSERT_EQ(children_seq_2.size(), 2);
  ASSERT_EQ(child_instr_seq_2.size(), 2);
  ASSERT_EQ(seq_2_children.size(), 2);
  EXPECT_EQ(children_seq_2[0]->GetInstruction(), seq_2_children[0]);
  EXPECT_EQ(children_seq_2[1]->GetInstruction(), seq_2_children[1]);
  EXPECT_EQ(child_instr_seq_2[0], seq_2_children[0]);
  EXPECT_EQ(child_instr_seq_2[1], seq_2_children[1]);

  // Test embedded inverter node of instruction tree
  auto children_inverter = children_seq_2[1]->GetChildren();
  auto child_instr_inverter = children_seq_2[1]->GetChildInstructions();
  auto inverter_children = seq_2_children[1]->ChildInstructions();
  ASSERT_EQ(children_inverter.size(), 1);
  ASSERT_EQ(child_instr_inverter.size(), 1);
  ASSERT_EQ(inverter_children.size(), 1);
  EXPECT_EQ(children_inverter[0]->GetInstruction(), inverter_children[0]);
  EXPECT_EQ(child_instr_inverter[0], inverter_children[0]);
}

TEST_F(InstructionTreeTest, FlattenlInstructionTree)
{
  // Test flattening of full instruction tree
  auto sequence = CreateTestInstructionTree();
  auto full_tree = CreateFullInstructionTree(sequence.get());
  auto full_flattened = FlattenBFS(full_tree);
  ASSERT_EQ(full_flattened.size(), 7);
}

TEST_F(InstructionTreeTest, GetLeavesInstructionTree)
{
  // Test getting leaves of full instruction tree
  auto sequence = CreateTestInstructionTree();
  auto full_tree = CreateFullInstructionTree(sequence.get());
  auto leaves = GetLeaves(full_tree);
  ASSERT_EQ(leaves.size(), 4);
}

TEST_F(InstructionTreeTest, CloneInstructionTree)
{
  // Clone the full instruction tree and check if the clone is correct
  auto sequence = CreateTestInstructionTree();
  auto full_tree = CreateFullInstructionTree(sequence.get());
  auto copy_tree = CloneInstructionTree(*full_tree.GetInstruction());
  ASSERT_EQ(full_tree.GetInstruction()->GetCategory(), copy_tree->GetCategory());
  ASSERT_EQ(full_tree.GetInstruction()->GetName(), copy_tree->GetName());
  ASSERT_EQ(full_tree.GetInstruction()->GetStringAttributes(), copy_tree->GetStringAttributes());
}

InstructionTreeTest::InstructionTreeTest() = default;

InstructionTreeTest::~InstructionTreeTest() = default;

// Test instruction tree:
// seq_1
// - wait_1
// - seq_2
//   - wait_2
//   - inverter
//     - wait_3
// - wait_4
std::unique_ptr<Instruction> CreateTestInstructionTree()
{
  auto seq_1 = GlobalInstructionRegistry().Create(kSequenceType);
  auto seq_2 = GlobalInstructionRegistry().Create(kSequenceType);
  auto inverter = GlobalInstructionRegistry().Create(kInverterType);
  auto wait_1 = GlobalInstructionRegistry().Create(kWaitType);
  auto wait_2 = GlobalInstructionRegistry().Create(kWaitType);
  auto wait_3 = GlobalInstructionRegistry().Create(kWaitType);
  auto wait_4 = GlobalInstructionRegistry().Create(kWaitType);

  AppendChildInstruction(*seq_1, std::move(wait_1));
  AppendChildInstruction(*seq_2, std::move(wait_2));
  AppendChildInstruction(*inverter, std::move(wait_3));
  AppendChildInstruction(*seq_2, std::move(inverter));
  AppendChildInstruction(*seq_1, std::move(seq_2));
  AppendChildInstruction(*seq_1, std::move(wait_4));

  return std::move(seq_1);
}
