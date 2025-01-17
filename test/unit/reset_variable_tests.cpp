/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
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

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

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
        <Copy inputVar="a" outputVar="target"/>
        <ResetVariable varName="target"/>
        <Copy inputVar="b" outputVar="target"/>
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
        <Copy inputVar="a" outputVar="target"/>
        <Copy inputVar="b" outputVar="target"/>
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
