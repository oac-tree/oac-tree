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
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

TEST(Include, Procedure_local)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence name=\"CountTwice\">\n"
    "        <Counter/>\n"
    "        <Counter/>\n"
    "    </Sequence>\n"
    "    <Repeat isRoot=\"true\" maxCount=\"10\">\n"
    "        <Include name=\"Counts\" path=\"CountTwice\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  if (status)
    {
      log_info("TEST(Include, Procedure_local) - Count is '%u'", sup::UnitTestHelper::CounterInstruction::GetCount());
      status = (20u == sup::UnitTestHelper::CounterInstruction::GetCount());
    }

  ASSERT_EQ(true, status);

}

TEST(Include, Procedure_param)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence name=\"CountParam\">\n"
    "        <Counter incr=\"$incr\"/>\n"
    "    </Sequence>\n"
    "    <Repeat isRoot=\"true\" maxCount=\"10\">\n"
    "        <Include name=\"Counts\" path=\"CountParam\" incr=\"2\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  if (status)
    {
      log_info("TEST(Include, Procedure_param) - Count is '%u'", sup::UnitTestHelper::CounterInstruction::GetCount());
      status = (20u == sup::UnitTestHelper::CounterInstruction::GetCount());
    }

  ASSERT_EQ(true, status);

}

TEST(Include, Procedure_undefined)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence isRoot=\"true\">\n"
    "        <Include name=\"undefined\" path=\"undefined\"/>\n"
    "        <Include name=\"undefined\" file=\"undefined\"/>\n"
    "        <Include name=\"undefined\" path=\"undefined\" file=\"undefined\"/>\n"
    "    </Sequence>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = (false == sup::UnitTestHelper::TryAndExecute(proc, &ui));

  ASSERT_EQ(true, status);

}

TEST(Include, Procedure_extern)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat isRoot=\"true\" maxCount=\"10\">\n"
    "        <Include name=\"Waits\" path=\"Parallel Wait\" file=\"../resources/parallel_sequence.xml\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
