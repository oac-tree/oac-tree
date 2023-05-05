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
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

TEST(For, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("For");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("elementVar", "i"));
  EXPECT_TRUE(instr->AddAttribute("arrayVar", "a"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(For, Registration)
{
  InstructionRegistry registry = GlobalInstructionRegistry();
  auto names = registry.RegisteredInstructionNames();
  auto it = std::find(names.begin(), names.end(), "For");
  ASSERT_TRUE(it != names.end());
}

TEST(For, Procedure_success)
{
  const std::string body{
    R"(
    <Sequence>
        <For elementVar="i" arrayVar="arr">
            <Increment varName="i"/>
        </For>
    </Sequence>
    <Workspace>
    <Local name="arr" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[2,4,6]"/>
    <Local name="i" type='{"type":"uint32"}' value='0' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(For, Procedure_type_failure)
{
  const std::string body{
    R"(
    <Sequence>
        <For elementVar="i" arrayVar="arr">
            <Increment varName="i"/>
        </For>
    </Sequence>
    <Workspace>
    <Local name="arr" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[2,4,6]"/>
    <Local name="i" type='{"type":"float32"}' value='0' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}
