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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

TEST(Increment, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Increment");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(Increment, IncrementSuccess)
{
  const std::string body{
    R"(
    <Sequence>
        <Increment varName="a"/>
        <Equals leftVar="a" rightVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='2' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Increment, IncrementFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <Increment varName="a"/>
        <Equals leftVar="a" rightVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='1' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Increment, IncrementFloatSuccess)
{
  const std::string body{
    R"(
    <Sequence>
        <Increment varName="a"/>
        <Equals leftVar="a" rightVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"float32"}' value='4' />
        <Local name="b" type='{"type":"float32"}' value='5' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Increment, IncrementStringFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <Increment varName="a"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"string"}' value='"fail"' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Increment, IncrementBoolFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <Increment varName="a"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"bool"}' value='true' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
