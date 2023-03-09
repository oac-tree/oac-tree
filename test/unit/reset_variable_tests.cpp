/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
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

TEST(ResetVariable, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("ResetVariable");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("varName", "var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(ResetVariable, CopyWithReset)
{
  const std::string body{R"(
    <Sequence>
        <Copy input="a" output="target"/>
        <ResetVariable varName="target"/>
        <Copy input="b" output="target"/>
    </Sequence>
    <Workspace>
        <Local name="target"/>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"string"}' value='"some name"' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(ResetVariable, CopyWithoutReset)
{
  const std::string body{R"(
    <Sequence>
        <Copy input="a" output="target"/>
        <Copy input="b" output="target"/>
    </Sequence>
    <Workspace>
        <Local name="target"/>
        <Local name="a" type='{"type":"uint8"}' value='1' />
        <Local name="b" type='{"type":"string"}' value='"some name"' />
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
