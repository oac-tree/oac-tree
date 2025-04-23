/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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

#include <sup/oac-tree/instructions/wait.h>

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::oac_tree;

TEST(Wait, Setup)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create("Wait");
  EXPECT_NO_THROW(instr->Setup(proc));

  EXPECT_TRUE(instr->AddAttribute("timeout", "five"));
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
}

TEST(Wait, InitialState)
{
  Wait instruction;
  EXPECT_EQ(instruction.ChildrenCount(), 0);

  EXPECT_FALSE(instruction.InsertInstruction(nullptr, 0));
  EXPECT_FALSE(instruction.TakeInstruction(0));
}

TEST(Wait, Registration)
{
  auto existing_names = GlobalInstructionRegistry().RegisteredInstructionNames();

  auto it = std::find(existing_names.begin(), existing_names.end(), "Wait");
  EXPECT_TRUE(it != existing_names.end());
}

TEST(Wait, Procedure_success)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Wait timeout="0.1"/>
  <Workspace/>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Wait, VariableTimeout)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Wait timeout="@mytimeout"/>
  <Workspace>
    <Local name="mytimeout" type='{"type":"float64"}' value='0.1'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(Wait, VariableTimeoutWrongType)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Wait timeout="@mytimeout"/>
  <Workspace>
    <Local name="mytimeout" type='{"type":"string"}' value='"1.0"'/>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(Wait, VariableTimeoutNotPresent)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Wait timeout="@mytimeout"/>
  <Workspace>
  </Workspace>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(Wait, SetupImpl_throw)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  const std::string procedure_body{
R"RAW(
  <Wait timeout="not-a-number"/>
  <Workspace/>
)RAW"};

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(procedure_body);
  auto proc = ParseProcedureString(procedure_string);
  EXPECT_THROW(sup::UnitTestHelper::TryAndExecute(proc, ui), InstructionSetupException);
}
