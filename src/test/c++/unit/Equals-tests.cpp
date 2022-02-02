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

#include "ExecutionStatus.h"
#include "InstructionRegistry.h"
#include "LogUI.h"
#include "UnitTestHelper.h"

#include <SequenceParser.h>
#include <gtest/gtest.h>

TEST(Equals, Equals_success)
{
  const std::string body{R"(
    <Equals lhs="a" rhs="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='1' />
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Equals, Equals_failure)
{
  const std::string body{R"(
    <Equals lhs="a" rhs="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='3' />
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail ..
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}
