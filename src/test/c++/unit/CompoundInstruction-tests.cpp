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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "CompoundInstruction.h"
#include "Wait.h"

#include <gtest/gtest.h>

using namespace sup::sequencer;

class CompoundInstructionTest : public ::testing::Test
{
public:
  class TestClass : public CompoundInstruction
  {
  public:
    TestClass() : CompoundInstruction("TestClass") {}
    ExecutionStatus ExecuteSingleImpl(UserInterface*, Workspace*) override { return {}; }
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

  EXPECT_EQ(compound.ChildInstructions(),
            std::vector<Instruction*>({child0, child2, child1}));

  // wrong insert index
  Wait child3;
  EXPECT_FALSE(compound.InsertInstruction(&child3, -1));
  EXPECT_FALSE(compound.InsertInstruction(&child3, 4));
}
