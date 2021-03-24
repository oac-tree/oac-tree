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

#include <gtest/gtest.h> // Google test framework

#include <common/BasicTypes.h>

#include <SequenceParser.h>

// Local header files

#include "UnitTestHelper.h"
#include "UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

TEST(Input, GetUserValue_success)
{

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence>\n"
    "        <Input description=\"Put some uint32 here\" output=\"uint32\"/>\n"
    "    </Sequence>\n"
    "    <Workspace>\n"
    "        <Local name=\"uint32\" type='{\"type\":\"uint32\"}'/>\n"
    "    </Workspace>\n"
    "</Procedure>");

  ccs::types::AnyValue value (1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  if (status)
    {
      status = (ccs::types::UnsignedInteger32 == ui.GetType());
    }

  // ToDo - Test workspace variable

  ASSERT_EQ(true, status);

}

TEST(Input, GetUserValue_failure)
{

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence>\n"
    "        <Input description=\"Put some uint32 here\" output=\"uint32\"/>\n"
    "    </Sequence>\n"
    "    <Workspace>\n"
    "        <Local name=\"uint32\" type='{\"type\":\"uint32\"}'/>\n"
    "    </Workspace>\n"
    "</Procedure>");

  ui.SetStatus(false);

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

TEST(Input, Variable_uninitialised)
{

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence>\n"
    "        <Input description=\"Put some uint32 here\" output=\"uint32\"/>\n"
    "    </Sequence>\n"
    "    <Workspace>\n"
    "        <Local name=\"uint32\"/>\n"
    "    </Workspace>\n"
    "</Procedure>");

  ccs::types::AnyValue value (1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

TEST(Input, Variable_undefined)
{

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence>\n"
    "        <Input description=\"Put some uint32 here\" output=\"uint32\"/>\n"
    "    </Sequence>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  ccs::types::AnyValue value (1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

// ToDo - Workspace variable not updated

#undef LOG_ALTERN_SRC
