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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/instruction_tree.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

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
  auto instr_1 = reinterpret_cast<Instruction*>(1u);
  auto instr_2 = reinterpret_cast<Instruction*>(2u);
  auto instr_3 = reinterpret_cast<Instruction*>(3u);

  // Test methods on tree existing of one element
  InstructionTree root_tree{instr_1};
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
  // Test root node of full instruction tree
  auto sequence = CreateTestInstructionTree();
  auto full_tree = CreateFullInstructionTree(sequence.get());
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

TEST_F(InstructionTreeTest, CreateNextInstructionTree)
{
  // Test root node of full instruction tree
  auto sequence = CreateTestInstructionTree();
  auto full_tree = CreateNextInstructionTree(sequence.get());
  EXPECT_EQ(full_tree.GetInstruction(), sequence.get());
  auto children_seq_1 = full_tree.GetChildren();
  auto child_instr_seq_1 = full_tree.GetChildInstructions();
  auto seq_1_children = sequence->ChildInstructions();
  ASSERT_EQ(children_seq_1.size(), 1);
  ASSERT_EQ(child_instr_seq_1.size(), 1);
  ASSERT_EQ(seq_1_children.size(), 3);
  EXPECT_EQ(children_seq_1[0]->GetInstruction(), seq_1_children[0]);
  EXPECT_EQ(child_instr_seq_1[0], seq_1_children[0]);
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

  AppendChildInstruction(*seq_1, wait_1.release());
  AppendChildInstruction(*seq_2, wait_2.release());
  AppendChildInstruction(*inverter, wait_3.release());
  AppendChildInstruction(*seq_2, inverter.release());
  AppendChildInstruction(*seq_1, seq_2.release());
  AppendChildInstruction(*seq_1, wait_4.release());

  return std::move(seq_1);
}