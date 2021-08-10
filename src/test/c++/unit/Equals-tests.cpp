/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2021 ITER Organization,
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
#include <gtest/gtest.h>  // Google test framework

// Local header files

#include "ExecutionStatus.h"
#include "InstructionRegistry.h"
#include "LogUI.h"
#include "UnitTestHelper.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

static const std::string EqualProcedureString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing no workspace for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Equals lhs="a" rhs="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='1' />
    </Workspace>
</Procedure>
)RAW";

static const std::string NotEqualProcedureString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing no workspace for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Equals lhs="a" rhs="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='3' />
    </Workspace>
</Procedure>
)RAW";

// Function definition

TEST(Equals, Equals_success)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(EqualProcedureString);

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);
}

TEST(Equals, Equals_failure)
{
  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(NotEqualProcedureString);

  bool status = sup::UnitTestHelper::TryAndExecute(
      proc, &ui,
      sup::sequencer::ExecutionStatus::FAILURE);  // Should have expect failure in Setup but the
                                                  // exception does not cause SetupImpl to fail ..

  ASSERT_EQ(true, status);
}

#undef LOG_ALTERN_SRC
