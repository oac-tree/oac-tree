/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

TEST(GreaterThan, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("GreaterThan");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("leftVar", "left_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("rightVar", "right_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(GreaterThan, GreaterThan_success)
{
  const std::string body{R"(
    <GreaterThan leftVar="a" rightVar="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint64"}' value='15' />
        <Local name="b" type='{"type":"uint64"}' value='10' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail ..
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST(GreaterThan, GreaterThan_failure)
{
  const std::string body{R"(
    <GreaterThan leftVar="a" rightVar="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint64"}' value='4' />
        <Local name="b" type='{"type":"uint64"}' value='10' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail ..
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(LessThan, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("LessThan");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("leftVar", "left_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("rightVar", "right_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(LessThan, LessThan_success)
{
  const std::string body{R"(
    <LessThan leftVar="a" rightVar="b"/>
    <Workspace>
        <Local name="a" type='{"type":"float64"}' value='5' />
        <Local name="b" type='{"type":"uint64"}' value='10' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail ..
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST(LessThan, LessThan_failure)
{
  const std::string body{R"(
    <LessThan leftVar="a" rightVar="b"/>
    <Workspace>
        <Local name="a" type='{"type":"uint64"}' value='24' />
        <Local name="b" type='{"type":"uint64"}' value='10' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc =
      ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  // Should have expect failure in Setup but the exception does not cause SetupImpl to fail ..
  EXPECT_TRUE(
      sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}





