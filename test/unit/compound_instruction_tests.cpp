/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/compound_instruction.h>
#include <sup/oac-tree/instructions/wait.h>

#include <gtest/gtest.h>

#include <sup/oac-tree/constants.h>

using namespace sup::oac_tree;

class CompoundInstructionTest : public ::testing::Test
{
public:
  class TestClass : public CompoundInstruction
  {
  public:
    TestClass() : CompoundInstruction("TestClass") {}
    ExecutionStatus ExecuteSingleImpl(UserInterface&, Workspace&) override { return {}; }
  };
};

TEST_F(CompoundInstructionTest, InsertChild)
{
  TestClass compound;
  EXPECT_TRUE(IsDefinedAttributeName(compound, Constants::IS_ROOT_ATTRIBUTE_NAME));
  EXPECT_TRUE(IsDefinedAttributeName(compound, Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(IsDefinedAttributeName(compound, Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME));
  EXPECT_EQ(compound.ChildrenCount(), 0);

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0));
  EXPECT_EQ(compound.ChildrenCount(), 1);

  auto child1 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(std::unique_ptr<Instruction>{child1}, 1));
  EXPECT_EQ(compound.ChildrenCount(), 2);

  // inserting child in between
  auto child2 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(std::unique_ptr<Instruction>{child2}, 1));
  EXPECT_EQ(compound.ChildrenCount(), 3);

  EXPECT_EQ(compound.ChildInstructions(), std::vector<Instruction*>({child0, child2, child1}));

  // wrong insert index
  EXPECT_FALSE(compound.InsertInstruction(nullptr, -1));
  EXPECT_FALSE(compound.InsertInstruction(nullptr, 4));
}

TEST_F(CompoundInstructionTest, TakeChild)
{
  TestClass compound;

  // inserting child
  auto child0 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0));
  EXPECT_EQ(compound.ChildrenCount(), 1);

  // removing child
  auto child0_taken = compound.TakeInstruction(0);
  EXPECT_EQ(child0, child0_taken.get());
  EXPECT_EQ(compound.ChildrenCount(), 0);
}

TEST_F(CompoundInstructionTest, TakeMiddleChild)
{
  TestClass compound;

  // inserting 4 children
  auto child0 = new Wait;
  compound.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0);
  auto child1 = new Wait;
  compound.InsertInstruction(std::unique_ptr<Instruction>{child1}, 1);
  auto child2 = new Wait;
  compound.InsertInstruction(std::unique_ptr<Instruction>{child2}, 2);
  auto child3 = new Wait;
  compound.InsertInstruction(std::unique_ptr<Instruction>{child3}, 3);

  // taking middle child
  auto child1_taken = compound.TakeInstruction(1);
  EXPECT_EQ(child1, child1_taken.get());
  EXPECT_EQ(compound.ChildrenCount(), 3);
  EXPECT_EQ(compound.ChildInstructions(), std::vector<Instruction*>({child0, child2, child3}));

  // attempt to take non-existing one
  EXPECT_TRUE(compound.TakeInstruction(3) == nullptr);
}
