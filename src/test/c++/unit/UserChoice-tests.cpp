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

// Global header files

#include <SequenceParser.h>
#include <common/BasicTypes.h>
#include <gtest/gtest.h>  // Google test framework

// Local header files

#include "UnitTestHelper.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

TEST(UserChoice, GetUserChoice)
{
  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
      "           name=\"Trivial procedure for testing purposes\"\n"
      "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
      "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
      "    <UserChoice description=\"Tick or wait\">\n"
      "        <Counter/>\n"
      "        <Counter incr=\"2\"/>\n"
      "        <Wait timeout=\"0.1\"/>\n"
      "        <Inverter>\n"
      "            <Counter/>\n"
      "        </Inverter>\n"
      "    </UserChoice>\n"
      "    <Workspace/>\n"
      "</Procedure>");

  ui.SetChoice(0);

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  // Instructions called

  if (status)
  {
    status = (1u == sup::UnitTestHelper::CounterInstruction::GetCount());
  }

  if (status)
  {
    ui.SetChoice(1);
    status = sup::UnitTestHelper::TryAndExecute(proc, &ui);
  }

  if (status)
  {
    status = (3u == sup::UnitTestHelper::CounterInstruction::GetCount());
  }

  if (status)
  {
    ui.SetChoice(2);
    status = sup::UnitTestHelper::TryAndExecute(proc, &ui);
  }

  if (status)
  {
    status = (3u == sup::UnitTestHelper::CounterInstruction::GetCount());
  }

  // Instruction called and return failure

  if (status)
  {
    ui.SetChoice(3);
    status =
        sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);
  }

  if (status)
  {
    status = (4u == sup::UnitTestHelper::CounterInstruction::GetCount());
  }

  // Invalid choices

  if (status)
  {
    ui.SetChoice(4);
    status =
        sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);
  }

  if (status)
  {
    ui.SetChoice(-1);
    status =
        sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);
  }

  ASSERT_EQ(true, status);
}

#undef LOG_ALTERN_SRC
