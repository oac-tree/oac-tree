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

#include "LogUI.h"
#include "UnitTestHelper.h"

#include <Instruction.h>
#include <InstructionRegistry.h>
#include <SequenceParser.h>
#include <common/BasicTypes.h>
#include <gtest/gtest.h>

// ToDo - Should implement test-specific instruction class to verify if called multiple times, or
// called by the ForceSuccess, etc.
//        Need for this a wait to get a reference to the instruction in the procedure.

TEST(Fallback, Procedure_first)
{
  const std::string body{R"(
    <Fallback>
        <ForceSuccess name="success">
            <Counter/>
        </ForceSuccess>
        <ForceSuccess name="success">
            <Counter/>
        </ForceSuccess>
    </Fallback>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 1);
}

TEST(Fallback, Procedure_alternative)
{
  const std::string body{R"(
    <Fallback>
        <Inverter name="failure">
            <Counter/>
        </Inverter>
        <ForceSuccess name="success">
            <Counter/>
        </ForceSuccess>
    </Fallback>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 2);
}

TEST(Fallback, Procedure_failure)
{
  const std::string body{R"(
    <Fallback>
        <Inverter name="failure">
            <Counter/>
        </Inverter>
        <Inverter name="failure">
            <Counter/>
        </Inverter>
    </Fallback>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 2);
}
