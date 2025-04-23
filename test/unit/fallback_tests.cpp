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

#include "unit_test_helper.h"

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

TEST(Fallback, Procedure_first)
{
  const std::string body{R"(
    <Fallback>
        <ForceSuccess name="success">
            <Counter/>
        </ForceSuccess>
        <ForceSuccess name="success">
            <Counter/>
        </ForceSuccess>
    </Fallback>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 1);
}

TEST(Fallback, Procedure_alternative)
{
  const std::string body{R"(
    <Fallback>
        <Inverter name="failure">
            <Counter/>
        </Inverter>
        <ForceSuccess name="success">
            <Counter/>
        </ForceSuccess>
    </Fallback>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 2);
}

TEST(Fallback, Procedure_failure)
{
  const std::string body{R"(
    <Fallback>
        <Inverter name="failure">
            <Counter/>
        </Inverter>
        <Inverter name="failure">
            <Counter/>
        </Inverter>
    </Fallback>
    <Workspace>
    </Workspace>
)"};

  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
  EXPECT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 2);
}
