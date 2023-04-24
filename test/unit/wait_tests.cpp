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

#include <sup/sequencer/instructions/wait.h>

#include "unit_test_helper.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

TEST(Wait, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Wait");
  EXPECT_NO_THROW(instr->Setup(proc));

  EXPECT_TRUE(instr->AddAttribute("timeout", "five"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
}

TEST(Wait, InitialState)
{
  Wait instruction;
  EXPECT_EQ(instruction.ChildrenCount(), 0);

  Wait child;
  EXPECT_FALSE(instruction.InsertInstruction(&child, 0));
  EXPECT_FALSE(instruction.TakeInstruction(0));
}

TEST(Wait, Registration)
{
  auto existing_names = GlobalInstructionRegistry().RegisteredInstructionNames();

  auto it = std::find(existing_names.begin(), existing_names.end(), "Wait");
  EXPECT_TRUE(it != existing_names.end());
}

TEST(Wait, Procedure_success)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Wait timeout=\"0.1\"/>\n"
      "    <Workspace/>\n"
      "</Procedure>");

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Wait, SetupImpl_throw)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Wait timeout=\"not-a-number\"/>\n"
      "    <Workspace/>\n"
      "</Procedure>");

  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail.
  EXPECT_THROW(sup::UnitTestHelper::TryAndExecute(proc, ui), InstructionSetupException);
}
