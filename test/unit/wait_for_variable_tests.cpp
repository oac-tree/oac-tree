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

#include <gtest/gtest.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

using namespace sup::oac_tree;

TEST(WaitForVariable, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("WaitForVariable");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_TRUE(instr->AddAttribute("timeout", "0"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(WaitForVariable, WaitForSingleVariableSuccess)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariable timeout="0.1" varName="a"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='3' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(WaitForVariable, WaitForVariableValueSuccess)
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
    <ParallelSequence name="parallel">
        <WaitForVariable timeout="0.5" varName="a" equalsVar="b"/>
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
    <ParallelSequence name="parallel">
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


TEST(WaitForVariable, WaitForVariableParallelMultipleSuccess)
{
  const std::string body{
    R"(
    <ParallelSequence name="parallel">
        <WaitForVariable timeout="4.0" varName="a" equalsVar="b"/>
        <Repeat maxCount="8">
            <Increment varName="a"/>
        </Repeat>
        <Repeat maxCount="2">
            <Decrement varName="b"/>
        </Repeat>
    </ParallelSequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='3' />
        <Local name="b" type='{"type":"uint8"}' value='13' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(WaitForVariable, VariableTimeout)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariable timeout="@mytimeout" varName="a"/>
    </Sequence>
    <Workspace>
        <Local name="mytimeout" type='{"type":"float64"}' value='0.1'/>
        <Local name="a" type='{"type":"uint8"}' value='3' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(WaitForVariable, VariableTimeoutWrongType)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariable timeout="@mytimeout" varName="a"/>
    </Sequence>
    <Workspace>
        <Local name="mytimeout" type='{"type":"string"}' value='"oops"'/>
        <Local name="a" type='{"type":"uint8"}' value='3' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(WaitForVariable, VariableTimeoutNotPresent)
{
  const std::string body{
      R"(
    <Sequence>
        <WaitForVariable timeout="@mytimeout" varName="a"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"uint8"}' value='3' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

