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

#include <sup/sequencer/instructions/decorator_instruction.h>

#include "log_ui.h"
#include "unit_test_helper.h"

#include <sup/sequencer/instructions/wait.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <common/BasicTypes.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class DecoratorInstructionTest : public ::testing::Test
{
public:
  class TestClass : public DecoratorInstruction
  {
  public:
    TestClass() : DecoratorInstruction("TestClass") {}
    ExecutionStatus ExecuteSingleImpl(UserInterface*, Workspace*) override { return {}; }
  };
};

// ToDo - Should implement test-specific instruction class to verify if called multiple times, or
// called by the ForceSuccess, etc.
//        Need for this a wait to get a reference to the instruction in the procedure.

TEST_F(DecoratorInstructionTest, InsertChild)
{
  TestClass decorator;
  EXPECT_EQ(decorator.ChildrenCount(), 0);

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(decorator.InsertInstruction(child0, 0));
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
  EXPECT_TRUE(decorator.InsertInstruction(child0, 0));
  EXPECT_EQ(decorator.ChildrenCount(), 1);

  // not possible to take index !=0
  EXPECT_FALSE(decorator.TakeInstruction(1));

  // tremoving child
  auto child0_taken = decorator.TakeInstruction(0);
  EXPECT_EQ(child0, child0_taken);
  delete child0_taken;

  EXPECT_EQ(decorator.ChildrenCount(), 0);
}

TEST_F(DecoratorInstructionTest, ChildInstructions)
{
  auto registry = sup::sequencer::GlobalInstructionRegistry();
  auto instruction = GlobalInstructionRegistry().Create("ForceSuccess");
  auto decorator = dynamic_cast<DecoratorInstruction*>(instruction.get());
  ASSERT_TRUE(decorator != nullptr);
  ASSERT_TRUE(decorator->ChildInstructions().empty());

  auto child = GlobalInstructionRegistry().Create("Wait");
  decorator->SetInstruction(child.release());
  ASSERT_FALSE(decorator->ChildInstructions().empty());
}

TEST_F(DecoratorInstructionTest, ForceSuccess_success)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <ForceSuccess name=\"success\">\n"
      "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
      "        </ForceSuccess>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST_F(DecoratorInstructionTest, ForceSuccess_failure)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <ForceSuccess name=\"success\">\n"
      "            <Inverter name=\"failure\">\n"
      "                <Wait name=\"wait\" timeout=\"0.1\"/>\n"
      "            </Inverter>\n"
      "        </ForceSuccess>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST_F(DecoratorInstructionTest, Inverter_success)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <Inverter name=\"success\">\n"
      "            <Inverter name=\"failure\">\n"
      "                <Wait name=\"wait\" timeout=\"0.1\"/>\n"
      "            </Inverter>\n"
      "        </Inverter>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST_F(DecoratorInstructionTest, Inverter_failure)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <Inverter name=\"failure\">\n"
      "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
      "        </Inverter>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui, ExecutionStatus::FAILURE));
}

TEST_F(DecoratorInstructionTest, BaseClass_halt)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Repeat maxCount=\"10\">\n"
      "        <ParallelSequence>\n"
      "            <ForceSuccess name=\"success\">\n"
      "                <Wait name=\"wait\" timeout=\"1.0\"/>\n"
      "            </ForceSuccess>\n"
      "            <Inverter name=\"failure\">\n"
      "                <Wait name=\"wait\" timeout=\"0.1\"/>\n"
      "            </Inverter>\n"
      "        </ParallelSequence>\n"
      "    </Repeat>\n"
      "    <Workspace>\n"
      "    </Workspace>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui, ExecutionStatus::FAILURE));
}
