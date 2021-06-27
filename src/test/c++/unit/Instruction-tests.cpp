/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
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

#include "Instruction.h"

#include <gtest/gtest.h>

using namespace sup::sequencer;

class InstructionTest : public ::testing::Test
{
public:
  class TestInstruction : public Instruction
  {
  public:
    TestInstruction() : Instruction("TestInstruction") {}
    ExecutionStatus ExecuteSingleImpl(UserInterface*, Workspace*) override { return {}; }
  };
};

TEST_F(InstructionTest, SetAttribute)
{
  TestInstruction instruction;

  // it is not possible to set attribute if it doesn't exist
  EXPECT_FALSE(instruction.SetAttribute("non-existing", "value"));

  // adding attribute and checking its value
  instruction.AddAttribute("key", "value");
  EXPECT_EQ(instruction.GetAttribute("key"), std::string("value"));

  // changind the value
  EXPECT_TRUE(instruction.SetAttribute("key", "new-value"));
  EXPECT_EQ(instruction.GetAttribute("key"), std::string("new-value"));
}
