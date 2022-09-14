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

#include <sup/sequencer/instructions/wait.h>

#include "log_ui.h"
#include "unit_test_helper.h"

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <common/BasicTypes.h>

#include <gtest/gtest.h>

#include <algorithm>

TEST(Wait, InitialState)
{
  sup::sequencer::Wait instruction;
  EXPECT_EQ(instruction.ChildrenCount(), 0);

  sup::sequencer::Wait child;
  EXPECT_FALSE(instruction.InsertInstruction(&child, 0));
  EXPECT_FALSE(instruction.TakeInstruction(0));
}

TEST(Wait, Registration)
{
  auto existing_names = sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();

  auto it = std::find(existing_names.begin(), existing_names.end(), "Wait");
  ASSERT_TRUE(it != existing_names.end());
}

TEST(Wait, Procedure_success)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Wait timeout=\"0.1\"/>\n"
      "    <Workspace/>\n"
      "</Procedure>");

  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Wait, SetupImpl_throw)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <Wait timeout=\"not-a-number\"/>\n"
      "    <Workspace/>\n"
      "</Procedure>");

  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail.
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}
