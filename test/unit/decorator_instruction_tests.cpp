/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Oac-Tree
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

#include <sup/oac-tree/decorator_instruction.h>

#include "unit_test_helper.h"

#include <sup/oac-tree/instructions/wait.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

#include <sup/oac-tree/constants.h>

using namespace sup::oac_tree;

class DecoratorInstructionTest : public ::testing::Test
{
public:
  class TestClass : public DecoratorInstruction
  {
  public:
    TestClass() : DecoratorInstruction("TestClass") {}
    ExecutionStatus ExecuteSingleImpl(UserInterface&, Workspace&) override { return {}; }
  };
};

TEST_F(DecoratorInstructionTest, InsertChild)
{
  TestClass decorator;
  EXPECT_TRUE(IsDefinedAttributeName(decorator, Constants::IS_ROOT_ATTRIBUTE_NAME));
  EXPECT_TRUE(IsDefinedAttributeName(decorator, Constants::NAME_ATTRIBUTE_NAME));
  EXPECT_TRUE(IsDefinedAttributeName(decorator, Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME));
  EXPECT_EQ(decorator.ChildrenCount(), 0);

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(decorator.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0));
  EXPECT_EQ(decorator.ChildrenCount(), 1);
}

TEST_F(DecoratorInstructionTest, TakeChild)
{
  TestClass decorator;
  EXPECT_EQ(decorator.ChildrenCount(), 0);

  // not possible to take non-existing child
  EXPECT_FALSE(decorator.TakeInstruction(0));

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(decorator.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0));
  EXPECT_EQ(decorator.ChildrenCount(), 1);

  // not possible to take index !=0
  EXPECT_FALSE(decorator.TakeInstruction(1));

  // tremoving child
  auto child0_taken = decorator.TakeInstruction(0);
  EXPECT_EQ(child0, child0_taken.get());

  EXPECT_EQ(decorator.ChildrenCount(), 0);
}

TEST_F(DecoratorInstructionTest, ChildInstructions)
{
  auto registry = GlobalInstructionRegistry();
  auto instruction = GlobalInstructionRegistry().Create("ForceSuccess");
  ASSERT_TRUE(instruction.get() != nullptr);
  EXPECT_TRUE(instruction->ChildInstructions().empty());

  auto child = GlobalInstructionRegistry().Create("Wait");
  EXPECT_TRUE(AppendChildInstruction(*instruction, std::move(child)));
  EXPECT_FALSE(instruction->ChildInstructions().empty());
}

TEST_F(DecoratorInstructionTest, ForceSuccess_success)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/oac-tree\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/oac-tree oac-tree.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <ForceSuccess name=\"success\">\n"
      "            <Wait name=\"wait\" />\n"
      "        </ForceSuccess>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(DecoratorInstructionTest, ForceSuccess_failure)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/oac-tree\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/oac-tree oac-tree.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <ForceSuccess name=\"success\">\n"
      "            <Inverter name=\"failure\">\n"
      "                <Wait name=\"wait\" />\n"
      "            </Inverter>\n"
      "        </ForceSuccess>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(DecoratorInstructionTest, Inverter_success)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/oac-tree\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/oac-tree oac-tree.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <Inverter name=\"success\">\n"
      "            <Inverter name=\"failure\">\n"
      "                <Wait name=\"wait\" />\n"
      "            </Inverter>\n"
      "        </Inverter>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST_F(DecoratorInstructionTest, Inverter_failure)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/oac-tree\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/oac-tree oac-tree.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <Inverter name=\"failure\">\n"
      "            <Wait name=\"wait\" />\n"
      "        </Inverter>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(DecoratorInstructionTest, BaseClass_halt)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/oac-tree\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/oac-tree oac-tree.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <ParallelSequence>\n"
      "            <ForceSuccess name=\"success\">\n"
      "                <Wait name=\"wait\" timeout=\"1.0\"/>\n"
      "            </ForceSuccess>\n"
      "            <Inverter name=\"failure\">\n"
      "                <Wait name=\"wait\" />\n"
      "            </Inverter>\n"
      "        </ParallelSequence>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
