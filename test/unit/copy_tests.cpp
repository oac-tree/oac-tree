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

#include <sup/sequencer/instructions/copy.h>

#include "unit_test_helper.h"

#include <sup/sequencer/variables/local_variable.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

TEST(Copy, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Copy");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("input", "in_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("output", "out_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(Copy, Success)
{
  const std::string body{R"(
    <Copy name="copy_test" input="in_var" output="out_var"/>
    <Workspace>
        <Local name="in_var"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
        <Local name="out_var"/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Copy, MissingInputVariable)
{
  const std::string body{R"(
    <Copy name="copy_test" input="in_var" output="out_var"/>
    <Workspace>
        <Local name="out_var"/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(Copy, MissingOutputVariable)
{
  const std::string body{R"(
    <Copy name="copy_test" input="in_var" output="out_var"/>
    <Workspace>
        <Local name="in_var"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(Copy, IncompatibleVariables)
{
  const std::string body{R"(
    <Copy name="copy_test" input="in_var" output="out_var"/>
    <Workspace>
        <Local name="in_var"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
        <Local name="out_var"
               type='{"type":"bool"}'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
