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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

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
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui));
  EXPECT_EQ(CounterInstruction::GetCount(), 1);

  ui.SetChoice(1);
  EXPECT_TRUE(TryAndExecute(proc, ui));
  EXPECT_EQ(CounterInstruction::GetCount(), 3);

  ui.SetChoice(2);
  EXPECT_TRUE(TryAndExecute(proc, ui));
  EXPECT_EQ(CounterInstruction::GetCount(), 3);

  // Instruction called and return failure
  ui.SetChoice(3);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  EXPECT_EQ(CounterInstruction::GetCount(), 4);

  // Invalid choices

  ui.SetChoice(4);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));

  ui.SetChoice(-1);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(UserChoice, VariableDescription)
{
  using sup::UnitTestHelper::CounterInstruction;
  using sup::UnitTestHelper::TryAndExecute;

  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
    <UserChoice description="@desc">
        <Wait/>
        <Wait/>
    </UserChoice>
    <Workspace>
        <Local name="desc" type='{"type":"string"}' value='"Hello"'/>
    </Workspace>
)"};

  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui));
}

TEST(UserChoice, VariableDescriptionWrongType)
{
  using sup::UnitTestHelper::CounterInstruction;
  using sup::UnitTestHelper::TryAndExecute;

  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
    <UserChoice description="@desc">
        <Wait/>
        <Wait/>
    </UserChoice>
    <Workspace>
        <Local name="desc" type='{"type":"float32"}' value='4.3'/>
    </Workspace>
)"};

  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(UserChoice, VariableDescriptionNotPresent)
{
  using sup::UnitTestHelper::CounterInstruction;
  using sup::UnitTestHelper::TryAndExecute;

  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
    <UserChoice description="@desc">
        <Wait/>
        <Wait/>
    </UserChoice>
    <Workspace>
    </Workspace>
)"};

  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
