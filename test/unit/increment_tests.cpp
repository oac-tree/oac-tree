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

#include "unit_test_helper.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

TEST(IncrementVariable, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("IncrementVariable");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(IncrementVariable, IncrementSuccess)
{
  const std::string body{
    R"(
    <Sequence>
        <IncrementVariable varName="a"/>
        <Equals lhs="a" rhs="b"/>
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

TEST(IncrementVariable, IncrementFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <IncrementVariable varName="a"/>
        <Equals lhs="a" rhs="b"/>
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

TEST(DecrementVariable, DecrementSuccess)
{
  const std::string body{
    R"(
    <Sequence>
        <DecrementVariable varName="a"/>
        <Equals lhs="a" rhs="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"float32"}' value='5' />
        <Local name="b" type='{"type":"float32"}' value='4' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(DecrementVariable, DecrementFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <DecrementVariable varName="a"/>
        <Equals lhs="a" rhs="b"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"float32"}' value='1' />
        <Local name="b" type='{"type":"float32"}' value='1' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
