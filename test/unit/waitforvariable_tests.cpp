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

#include <gtest/gtest.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

using namespace sup::sequencer;

TEST(WaitForVariable, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("WaitForVariable");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_TRUE(instr->AddAttribute("timeout", "0"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(WaitForVariable, WaitForVariableSuccess)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariable timeout="0.1" varName="a" equalsVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='3' />
        <Local name="b" type='{"type":"uint8"}' value='3' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(WaitForVariable, WaitForVariableParallelSuccess)
{
  const std::string body{
      R"(
    <ParallelSequence name="parallel" successThreshold="2" failureThreshold="1">
        <WaitForVariable timeout="0.1" varName="a" equalsVar="b"/>
        <Increment varName="a"/>
    </ParallelSequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='3' />
        <Local name="b" type='{"type":"uint8"}' value='4' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(WaitForVariable, WaitForVariableParallelFailure)
{
  const std::string body{
      R"(
    <ParallelSequence name="parallel" successThreshold="2" failureThreshold="1">
        <WaitForVariable timeout="0.1" varName="a" equalsVar="b"/>
        <Increment varName="a"/>
    </ParallelSequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='0' />
        <Local name="b" type='{"type":"uint8"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
