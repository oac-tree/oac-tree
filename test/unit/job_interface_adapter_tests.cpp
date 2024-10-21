/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sup/sequencer/job_interface_adapter.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Exactly;

using namespace sup::sequencer;

class JobInterfaceAdapterTest : public ::testing::Test
{
protected:
  JobInterfaceAdapterTest() = default;
  virtual ~JobInterfaceAdapterTest() = default;

  sup::UnitTestHelper::MockJobInfoIO m_test_job_info_io;
};

TEST_F(JobInterfaceAdapterTest, Construction)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, JobStateUpdated(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, PutValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserChoice(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Message(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Log(_, _)).Times(Exactly(0));

  const std::string kTestPrefix = "AutomationJobInterfaceTests:Construction:";
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  JobInterfaceAdapter job_interface_adapter{*proc, m_test_job_info_io};
  EXPECT_THROW(job_interface_adapter.GetInstructionMap(), InvalidOperationException);
  EXPECT_THROW(job_interface_adapter.GetOrderedInstructions(), InvalidOperationException);
}

TEST_F(JobInterfaceAdapterTest, InitializeInstructionTree)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, JobStateUpdated(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, PutValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserChoice(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Message(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Log(_, _)).Times(Exactly(0));

  const std::string kTestPrefix = "AutomationJobInterfaceTests:Construction:";
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
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
