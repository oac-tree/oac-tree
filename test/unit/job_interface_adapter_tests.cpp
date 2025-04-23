/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
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

#include "unit_test_helper.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/job_interface_adapter.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgReferee;

using namespace sup::oac_tree;

class JobInterfaceAdapterTest : public ::testing::Test
{
protected:
  JobInterfaceAdapterTest() = default;
  virtual ~JobInterfaceAdapterTest() = default;

  using StrictMockJobInfoIO = ::testing::StrictMock<sup::UnitTestHelper::MockJobInfoIO>;
  StrictMockJobInfoIO m_test_job_info_io;
};

TEST_F(JobInterfaceAdapterTest, Construction)
{
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  EXPECT_THROW(job_interface_adapter.GetInstructionMap(), InvalidOperationException);
  EXPECT_THROW(job_interface_adapter.GetOrderedInstructions(), InvalidOperationException);
}

TEST_F(JobInterfaceAdapterTest, InitializeInstructionTree)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  job_interface_adapter.InitializeInstructionTree(proc->RootInstruction());
  const auto& instr_map = job_interface_adapter.GetInstructionMap();
  EXPECT_EQ(instr_map.GetNumberOfInstructions(), 3);
  auto ordered_instr = job_interface_adapter.GetOrderedInstructions();
  EXPECT_EQ(ordered_instr.size(), 3);
  // Check that the ordered list is consistent with the instruction map
  sup::dto::uint32 instr_idx = 0;
  for (const auto* instr : ordered_instr)
  {
    EXPECT_EQ(instr_map.FindInstructionIndex(instr), instr_idx);
    ++instr_idx;
  }
}

TEST_F(JobInterfaceAdapterTest, UpdateInstructionStatus)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  {
    InSequence seq;
    InstructionState expected{ false, ExecutionStatus::NOT_STARTED };
    EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(0, expected)).Times(Exactly(1));
    EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(1, expected)).Times(Exactly(1));
    EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(2, expected)).Times(Exactly(1));
  }

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  job_interface_adapter.InitializeInstructionTree(proc->RootInstruction());
  const auto& instr_map = job_interface_adapter.GetInstructionMap();
  EXPECT_EQ(instr_map.GetNumberOfInstructions(), 3);
  auto ordered_instr = job_interface_adapter.GetOrderedInstructions();
  ASSERT_EQ(ordered_instr.size(), 3);
  job_interface_adapter.UpdateInstructionStatus(ordered_instr[0]);
  job_interface_adapter.UpdateInstructionStatus(ordered_instr[1]);
  job_interface_adapter.UpdateInstructionStatus(ordered_instr[2]);
}

TEST_F(JobInterfaceAdapterTest, VariableUpdated)
{
  sup::dto::AnyValue updated_value{ sup::dto::UnsignedInteger32Type, 42u };
  {
    InSequence seq;
    EXPECT_CALL(m_test_job_info_io, VariableUpdated(0, updated_value, true)).Times(Exactly(1));
    EXPECT_CALL(m_test_job_info_io, VariableUpdated(1, updated_value, true)).Times(Exactly(1));
    EXPECT_CALL(m_test_job_info_io, VariableUpdated(2, updated_value, true)).Times(Exactly(1));
  }

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  job_interface_adapter.VariableUpdated("one", updated_value, true);
  job_interface_adapter.VariableUpdated("var1", updated_value, true);
  job_interface_adapter.VariableUpdated("var2", updated_value, true);
}

TEST_F(JobInterfaceAdapterTest, OutputValues)
{
  const std::string description = "This is an output value";
  sup::dto::AnyValue output_value{ sup::dto::UnsignedInteger32Type, 42u };
  const std::string message = "My message";
  const std::string log_message = "A log message";
  EXPECT_CALL(m_test_job_info_io, PutValue(output_value, description)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, Message(message)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, Log(2, log_message)).Times(Exactly(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  job_interface_adapter.PutValue(output_value, description);
  job_interface_adapter.Message(message);
  job_interface_adapter.Log(2, log_message);
}

TEST_F(JobInterfaceAdapterTest, UserInput)
{
  const std::string description = "Provide a user value";
  sup::dto::AnyValue user_value{ sup::dto::UnsignedInteger32Type, 42u };
  sup::dto::AnyValue return_value{};
  const std::vector<std::string> choices{ "vanilla", "strawberry", "chocolate"};
  sup::dto::AnyValue metadata{ sup::dto::StringType, "Choose your favorite flavour"};
  int user_choice = 2;
  EXPECT_CALL(m_test_job_info_io, GetUserValue(_, return_value, description)).Times(Exactly(1))
    .WillOnce(DoAll(SetArgReferee<1>(user_value), Return(true)));
  EXPECT_CALL(m_test_job_info_io, GetUserChoice(_, choices, metadata)).Times(Exactly(1))
    .WillOnce(Return(user_choice));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  auto [retrieved, value] = GetBlockingUserValue(job_interface_adapter, return_value, description);
  EXPECT_TRUE(retrieved);
  EXPECT_EQ(value, user_value);
  auto [fetched, choice] = GetBlockingUserChoice(job_interface_adapter, choices, metadata);
  EXPECT_TRUE(fetched);
  EXPECT_EQ(choice, user_choice);
}

TEST_F(JobInterfaceAdapterTest, OnStateChange)
{
  JobState job_state = JobState::kSucceeded;
  EXPECT_CALL(m_test_job_info_io, JobStateUpdated(job_state)).Times(Exactly(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  job_interface_adapter.OnStateChange(job_state);
}

TEST_F(JobInterfaceAdapterTest, OnBreakpointChange)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  {
    InSequence seq;
    InstructionState expected{ true, ExecutionStatus::NOT_STARTED };
    EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(0, expected)).Times(Exactly(1));
    EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(2, expected)).Times(Exactly(1));
  }

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  job_interface_adapter.InitializeInstructionTree(proc->RootInstruction());
  const auto& instr_map = job_interface_adapter.GetInstructionMap();
  EXPECT_EQ(instr_map.GetNumberOfInstructions(), 3);
  auto ordered_instr = job_interface_adapter.GetOrderedInstructions();
  ASSERT_EQ(ordered_instr.size(), 3);
  job_interface_adapter.OnBreakpointChange(ordered_instr[0], true);
  job_interface_adapter.OnBreakpointChange(ordered_instr[2], true);
}

TEST_F(JobInterfaceAdapterTest, OnProcedureTick)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(Exactly(1));
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  // OnProcedureTick throws when the adapter still has no map for the instructions:
  EXPECT_THROW(job_interface_adapter.OnProcedureTick(*proc), InvalidOperationException);
  job_interface_adapter.InitializeInstructionTree(proc->RootInstruction());
  job_interface_adapter.OnProcedureTick(*proc);
}
