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

#include <sup/sequencer/SequenceParser.h>

#include <common/BasicTypes.h>

#include <gtest/gtest.h>

TEST(UserChoice, GetUserChoice)
{
  using sup::UnitTestHelper::CounterInstruction;
  using sup::UnitTestHelper::TryAndExecute;

  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
    <UserChoice description="Tick or wait">
        <Counter/>
        <Counter incr="2"/>
        <Wait timeout="0.1"/>
        <Inverter>
            <Counter/>
        </Inverter>
    </UserChoice>
    <Workspace/>
)"};

  auto proc =
      sup::sequencer::ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, &ui));
  EXPECT_EQ(CounterInstruction::GetCount(), 1);

  ui.SetChoice(1);
  EXPECT_TRUE(TryAndExecute(proc, &ui));
  EXPECT_EQ(CounterInstruction::GetCount(), 3);

  ui.SetChoice(2);
  EXPECT_TRUE(TryAndExecute(proc, &ui));
  EXPECT_EQ(CounterInstruction::GetCount(), 3);

  // Instruction called and return failure
  ui.SetChoice(3);
  EXPECT_TRUE(TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
  EXPECT_EQ(CounterInstruction::GetCount(), 4);

  // Invalid choices

  ui.SetChoice(4);
  EXPECT_TRUE(TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));

  ui.SetChoice(-1);
  EXPECT_TRUE(TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}
