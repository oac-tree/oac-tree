/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

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
