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

#include "UnitTestHelper.h"
#include "UserInterface.h"

#include <SequenceParser.h>
#include <common/BasicTypes.h>
#include <gtest/gtest.h>

TEST(Input, GetUserValue_success)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="uint32" type='{"type":"uint32"}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ccs::types::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_TRUE(ccs::types::UnsignedInteger32 == ui.GetType());

  // ToDo - Test workspace variable
}

TEST(Input, GetUserValue_failure)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="uint32" type='{"type":"uint32"}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ui.SetStatus(false);
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

TEST(Input, Variable_uninitialised)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
    </Sequence>
    <Workspace>
        <Local name="uint32"/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ccs::types::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

TEST(Input, Variable_undefined)
{
  const std::string body{R"(
    <Sequence>
        <Input description="Put some uint32 here" output="uint32"/>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ccs::types::AnyValue value(1234u);
  ui.SetStatus(true);
  ui.SetValue(value);

  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

// ToDo - Workspace variable not updated
