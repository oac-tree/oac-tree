/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "unit_test_helper.h"

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

#include <sup/oac-tree/constants.h>

using namespace sup::oac_tree;

class InstructionTest : public ::testing::Test
{
public:
  class TestInstruction : public Instruction
  {
  public:
    TestInstruction() : Instruction("TestInstruction") {}
    ExecutionStatus ExecuteSingleImpl(UserInterface&, Workspace&) override
    {
      return ExecutionStatus::SUCCESS;
    }
  };
};

TEST_F(InstructionTest, SetAttribute)
{
  TestInstruction instruction;
  EXPECT_TRUE(IsDefinedAttributeName(instruction, Constants::IS_ROOT_ATTRIBUTE_NAME));
  EXPECT_TRUE(IsDefinedAttributeName(instruction, Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_FALSE(IsDefinedAttributeName(instruction, Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME));

  // it is not possible to set attribute if it doesn't exist
  EXPECT_FALSE(instruction.SetAttribute("non-existing", "value"));

  // adding attribute and checking its value
  instruction.AddAttribute("key", "value");
  EXPECT_EQ(instruction.GetAttributeString("key"), std::string("value"));

  // changind the value
  EXPECT_TRUE(instruction.SetAttribute("key", "new-value"));
  EXPECT_EQ(instruction.GetAttributeString("key"), std::string("new-value"));
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

TEST_F(InstructionTest, Breakpoints)
{
  sup::UnitTestHelper::EmptyUserInterface ui{};
  Workspace ws{};
  {
    // Test mechanism of hitting and acknowledging breakpoints
    TestInstruction instruction;
    // Breakpoint is never hit when it was not executed
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());
    instruction.SetBreakpoint();
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());

    // Executing the instruction hits the breakpoint
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    instruction.ExecuteSingle(ui, ws);
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    // Acknowledging the breakpoint is only true once
    EXPECT_TRUE(instruction.AcknowledgeBreakpoint());
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());

    // Executing when a breakpoint was acknowledged will execute it properly
    instruction.ExecuteSingle(ui, ws);
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::SUCCESS);
  }
  {
    // Test mechanism of resetting breakpoints
    TestInstruction instruction;
    // Breakpoint is never hit when it was not executed
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());
    instruction.SetBreakpoint();
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());
    instruction.Reset(ui);
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());

    // Executing the instruction hits the breakpoint, but reset puts it back to 'set'
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    instruction.ExecuteSingle(ui, ws);
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    instruction.Reset(ui);
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);

    // Executing, acknowledging and resetting the breakpoint will result in it being hit again
    // on the next execution
    instruction.ExecuteSingle(ui, ws);
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    EXPECT_TRUE(instruction.AcknowledgeBreakpoint());
    instruction.Reset(ui);
    EXPECT_FALSE(instruction.AcknowledgeBreakpoint());
    instruction.ExecuteSingle(ui, ws);
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::NOT_STARTED);
    EXPECT_TRUE(instruction.AcknowledgeBreakpoint());
    // Only without the reset will execution be able to proceed
    instruction.ExecuteSingle(ui, ws);
    EXPECT_EQ(instruction.GetStatus(), ExecutionStatus::SUCCESS);
  }
}
