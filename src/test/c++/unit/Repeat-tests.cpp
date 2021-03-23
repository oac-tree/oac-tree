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

#include <algorithm> // std::find

#include <gtest/gtest.h> // Google test framework

#include <common/BasicTypes.h>

#include <SequenceParser.h>

#include <Instruction.h>
#include <InstructionRegistry.h>

// Local header files

#include "UnitTestHelper.h"
#include "LogUI.h"
#include "CounterInstruction.h"

#include "InstructionRegistry.h"
#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

TEST(Repeat, Registration)
{

  sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
  bool status = (registry.RegisteredInstructionNames().end() != std::find(registry.RegisteredInstructionNames().begin(), registry.RegisteredInstructionNames().end(), "Repeat"));

  ASSERT_EQ(true, status);

}

TEST(Repeat, Procedure_success)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Counter/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui);

  if (status)
    {
      log_info("TEST(Repeat, Procedure_success) - Count is '%u'", sup::sequencer::CounterInstruction::GetCount());
      status = (10u == sup::sequencer::CounterInstruction::GetCount());
    }

  ASSERT_EQ(true, status);

}

TEST(Repeat, Procedure_failure)
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
    "            <Counter/>\n"
    "        </Inverter>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  if (status)
    {
      log_info("TEST(Repeat, Procedure_failure) - Count is '%u'", sup::sequencer::CounterInstruction::GetCount());
      status = (1u == sup::sequencer::CounterInstruction::GetCount());
    }

  ASSERT_EQ(true, status);

}

TEST(Repeat, Procedure_attribute)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"undefined\">\n"
    "        <Counter/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = (false == TryAndExecute(proc, &ui)); // Expect failure during Setup

  if (status)
    {
      status = (0u == sup::sequencer::CounterInstruction::GetCount());
    }

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
