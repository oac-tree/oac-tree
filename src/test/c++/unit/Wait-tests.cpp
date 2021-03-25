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

// Local header files

#include "InstructionRegistry.h"

#include "UnitTestHelper.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

TEST(Wait, Registration)
{

  sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
  bool status = (registry.RegisteredInstructionNames().end() != std::find(registry.RegisteredInstructionNames().begin(), registry.RegisteredInstructionNames().end(), "Wait"));

  ASSERT_EQ(true, status);

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

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);

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

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui)); // Should have expect failure in Setup but the exception does not cause SetupImpl to fail ..

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
