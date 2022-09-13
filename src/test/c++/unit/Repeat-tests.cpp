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

#include "LogUI.h"
#include "UnitTestHelper.h"

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <common/BasicTypes.h>

#include <gtest/gtest.h>

#include <algorithm>

TEST(Repeat, Registration)
{
  sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
  auto names = registry.RegisteredInstructionNames();
  auto it = std::find(names.begin(), names.end(), "Repeat");
  ASSERT_TRUE(it != names.end());
}

TEST(Repeat, Procedure_success)
{
  const std::string body{R"(
    <Repeat maxCount="10">
        <Counter/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 10);
}

TEST(Repeat, Procedure_failure)
{
  const std::string body{R"(
    <Repeat maxCount="10">
        <Inverter name="failure">
            <Counter/>
        </Inverter>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 1);
}

TEST(Repeat, Procedure_attribute)
{
  const std::string body{R"(
    <Repeat maxCount="undefined">
        <Counter/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  // Expect failure during Setup
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 0);
}
