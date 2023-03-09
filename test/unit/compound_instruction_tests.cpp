/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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

#include <sup/sequencer/instructions/compound_instruction.h>
#include <sup/sequencer/instructions/wait.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

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
  EXPECT_EQ(compound.ChildrenCount(), 0);

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(child0, 0));
  EXPECT_EQ(compound.ChildrenCount(), 1);

  auto child1 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(child1, 1));
  EXPECT_EQ(compound.ChildrenCount(), 2);

  // inserting child in between
  auto child2 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(child2, 1));
  EXPECT_EQ(compound.ChildrenCount(), 3);

  EXPECT_EQ(compound.ChildInstructions(), std::vector<Instruction*>({child0, child2, child1}));

  // wrong insert index
  Wait child3;
  EXPECT_FALSE(compound.InsertInstruction(&child3, -1));
  EXPECT_FALSE(compound.InsertInstruction(&child3, 4));
}

TEST_F(CompoundInstructionTest, TakeChild)
{
  TestClass compound;

  // inserting child
  auto child0 = new Wait;
  EXPECT_TRUE(compound.InsertInstruction(child0, 0));
  EXPECT_EQ(compound.ChildrenCount(), 1);

  // removing child
  auto child0_taken = compound.TakeInstruction(0);
  EXPECT_EQ(child0, child0_taken);
  EXPECT_EQ(compound.ChildrenCount(), 0);
  delete child0_taken;
}

TEST_F(CompoundInstructionTest, TakeMiddleChild)
{
  TestClass compound;

  // inserting 4 children
  auto child0 = new Wait;
  compound.InsertInstruction(child0, 0);
  auto child1 = new Wait;
  compound.InsertInstruction(child1, 1);
  auto child2 = new Wait;
  compound.InsertInstruction(child2, 2);
  auto child3 = new Wait;
  compound.InsertInstruction(child3, 3);

  // taking middle child
  auto child1_taken = compound.TakeInstruction(1);
  EXPECT_EQ(child1, child1_taken);
  EXPECT_EQ(compound.ChildrenCount(), 3);
  EXPECT_EQ(compound.ChildInstructions(), std::vector<Instruction*>({child0, child2, child3}));
  delete child1_taken;

  // attempt to take non-existing one
  EXPECT_TRUE(compound.TakeInstruction(3) == nullptr);
}
