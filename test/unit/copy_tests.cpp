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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/instructions/copy.h>

#include "unit_test_helper.h"

#include <sup/oac-tree/variables/local_variable.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

TEST(Copy, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Copy");
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  EXPECT_TRUE(instr->AddAttribute("inputVar", "in_var"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instr->AddAttribute("outputVar", "out_var"));
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(Copy, Success)
{
  const std::string body{R"(
    <Copy name="copy_test" inputVar="in_var" outputVar="out_var"/>
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

TEST(Copy, ArrayCopy)
{
  const std::string body{R"(
    <Sequence>
        <Copy name="copy_test" inputVar="in_var[1]" outputVar="out_var[1]"/>
        <Equals leftVar="out_var[1]" rightVar="in_var[1]"/>
        <Equals leftVar="out_var[1]" rightVar="two"/>
        <Copy name="copy_test" inputVar="in_var[0]" outputVar="out_var[0]"/>
        <Equals leftVar="out_var[0]" rightVar="in_var[0]"/>
        <Equals leftVar="out_var" rightVar="in_var"/>
    </Sequence>
    <Workspace>
        <Local name="in_var"
               type='{"type":"sup::test::MyArray/v1.0","multiplicity":2,"element":{"type":"uint16"}}'
               value='[1,2]'/>
        <Local name="out_var"
               type='{"type":"sup::test::MyArray/v1.0","multiplicity":2,"element":{"type":"uint16"}}'
               value='[0,0]'/>
        <Local name="two" type='{"type":"uint16"}' value='2'/>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Copy, MissingInputVariable)
{
  const std::string body{R"(
    <Copy name="copy_test" inputVar="in_var" outputVar="out_var"/>
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
    <Copy name="copy_test" inputVar="in_var" outputVar="out_var"/>
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
    <Copy name="copy_test" inputVar="in_var" outputVar="out_var"/>
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
