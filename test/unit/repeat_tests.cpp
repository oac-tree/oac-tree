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
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

TEST(Repeat, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Repeat");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  auto wait = GlobalInstructionRegistry().Create("Wait");
  ASSERT_TRUE(AppendChildInstruction(*instr, std::move(wait)));
  EXPECT_NO_THROW(instr->Setup(proc));

  EXPECT_TRUE(instr->AddAttribute("maxCount", "five"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
}

TEST(Repeat, Registration)
{
  InstructionRegistry registry = GlobalInstructionRegistry();
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

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
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

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
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

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  // Expect failure during Setup
  EXPECT_THROW(sup::UnitTestHelper::TryAndExecute(proc, ui), InstructionSetupException);
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 0);
}
