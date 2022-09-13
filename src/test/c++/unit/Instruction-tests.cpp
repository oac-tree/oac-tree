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

#include <sup/sequencer/Instruction.h>

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

TEST_F(InstructionTest, SetName)
{
  TestInstruction instruction;
  const std::string name_of_attribute{"name"};

  // by default there is no such attribute, and name is empty
  EXPECT_EQ(instruction.GetName(), std::string());
  EXPECT_FALSE(instruction.HasAttribute(name_of_attribute));

  // it is possible to set name, attribute should appear
  const std::string expected_name{"expected_name"};
  instruction.SetName(expected_name);
  EXPECT_EQ(instruction.GetName(), expected_name);
  EXPECT_TRUE(instruction.HasAttribute(name_of_attribute));

  // it should be possible to change name again
  const std::string new_name{"new_name"};
  instruction.SetName(new_name);
  EXPECT_EQ(instruction.GetName(), new_name);
  EXPECT_TRUE(instruction.HasAttribute(name_of_attribute));
}
