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
#include <sup/oac-tree/job_map.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::oac_tree;

class JobMapTest : public ::testing::Test
{
protected:
  JobMapTest();
  virtual ~JobMapTest();
};

TEST_F(JobMapTest, Construction)
{
  // Create procedure
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);

  // Create JobMap (no instruction mapping yet)
  JobMap job_map{*proc};
  EXPECT_EQ(job_map.GetNumberOfInstructions(), 0);
  EXPECT_THROW(job_map.GetInstructionIndex(proc->RootInstruction()), InvalidOperationException);
  EXPECT_THROW(job_map.GetInstructionMap(), InvalidOperationException);
  EXPECT_EQ(job_map.GetNumberOfVariables(), 3);
  EXPECT_EQ(job_map.GetVariableIndex("one"), 0);
  EXPECT_EQ(job_map.GetVariableIndex("var1"), 1);
  EXPECT_EQ(job_map.GetVariableIndex("var2"), 2);
  const auto& var_map = job_map.GetVariableMap();
  EXPECT_EQ(var_map.GetNumberOfVariables(), 3);

  // Initialize instruction tree will provide instruction mapping
  job_map.InitializeInstructionTree(proc->RootInstruction());
  EXPECT_EQ(job_map.GetNumberOfInstructions(), 3);
  EXPECT_EQ(job_map.GetInstructionIndex(proc->RootInstruction()), 0);
  const auto& instr_map = job_map.GetInstructionMap();
  EXPECT_EQ(instr_map.GetNumberOfInstructions(), 3);
  EXPECT_EQ(job_map.GetNumberOfVariables(), 3);
  auto ordered_instructions = job_map.GetOrderedInstructions();
  EXPECT_EQ(ordered_instructions, GetReverseMap(instr_map.GetInstructionIndexMap()));
}

JobMapTest::JobMapTest() = default;

JobMapTest::~JobMapTest() = default;
