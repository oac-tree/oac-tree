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
#include <sup/oac-tree/instruction_utils.h>
#include <sup/oac-tree/job_info.h>
#include <sup/oac-tree/local_job.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <mutex>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::Return;

using namespace sup::oac_tree;

class TestJobInfoIO : public sup::oac_tree::IJobInfoIO
{
public:
  TestJobInfoIO();
  ~TestJobInfoIO() override;
  MOCK_METHOD(void, InitNumberOfInstructions, (sup::dto::uint32), (override));
  MOCK_METHOD(void, InstructionStateUpdated, (sup::dto::uint32, sup::oac_tree::InstructionState), (override));
  MOCK_METHOD(void, VariableUpdated, (sup::dto::uint32, const sup::dto::AnyValue&, bool), (override));
  MOCK_METHOD(void, PutValue, (const sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(bool, GetUserValue, (sup::dto::uint64, sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(int, GetUserChoice, (sup::dto::uint64, const std::vector<std::string>&, const sup::dto::AnyValue&), (override));
  MOCK_METHOD(void, Interrupt, (sup::dto::uint64), (override));
  MOCK_METHOD(void, Message, (const std::string&), (override));
  MOCK_METHOD(void, Log, (int, const std::string&), (override));
  MOCK_METHOD(void, NextInstructionsUpdated, (const std::vector<sup::dto::uint32>&), (override));
  void JobStateUpdated(JobState state) override;

  bool WaitForJobState(JobState state, double timeout_sec);

private:
  JobState m_job_state;
  std::mutex m_mtx;
  std::condition_variable m_cv;
};

class LocalJobTest : public ::testing::Test
{
protected:
  LocalJobTest() = default;
  virtual ~LocalJobTest() = default;

  using StrictTestJobInfoIO = ::testing::StrictMock<TestJobInfoIO>;
  StrictTestJobInfoIO m_test_job_info_io;
};

TEST_F(LocalJobTest, Construction)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(3));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  JobState initial_job_state = JobState::kInitial;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(initial_job_state, 0.0));
}

TEST_F(LocalJobTest, GetInfo)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(3));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  auto job_info = job.GetInfo();
  EXPECT_EQ(job_info.GetProcedureName(), "Common header");
  EXPECT_EQ(job_info.GetNumberOfVariables(), 3);
  EXPECT_EQ(job_info.GetNumberOfInstructions(), 3);
}

TEST_F(LocalJobTest, Start)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  // Every instruction does: NOT_STARTED -> NOT_FINISHED -> SUCCESS:
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(6));
  // All variables get an initial update and two of them are overwritten:
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(5));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(AtLeast(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  job.Start();
  JobState successful_job_state = JobState::kSucceeded;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(successful_job_state, 1.0));
}

TEST_F(LocalJobTest, Step)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  // Sequence has 2 updates: NOT_STARTED -> NOT_FINISHED
  // First Copy has 3 updates: NOT_STARTED -> NOT_FINISHED -> SUCCESS
  // Second Copy has no updates
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(3));
  // All variables get an initial update and one of them is overwritten:
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(4));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(AtLeast(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  job.Step();
  JobState paused_job_state = JobState::kPaused;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(paused_job_state, 1.0));
  job.Pause();
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(paused_job_state, 0.0));
}

TEST_F(LocalJobTest, HaltReset)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  // Sequence has 2 updates: NOT_STARTED -> NOT_FINISHED -> NOT_STARTED
  // First Copy has 3 updates: NOT_STARTED -> NOT_FINISHED -> SUCCESS -> NOT_STARTED
  // Second Copy has no updates
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(5));
  // All variables get an initial update and one of them is overwritten; upon reset,
  // they will all again receive updates of their initial values
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(7));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(AtLeast(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  job.Step();
  JobState paused_job_state = JobState::kPaused;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(paused_job_state, 1.0));
  job.Halt();
  JobState halted_job_state = JobState::kHalted;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(halted_job_state, 1.0));
  job.Reset();
  JobState initial_job_state = JobState::kInitial;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(initial_job_state, 1.0));
}

TEST_F(LocalJobTest, Breakpoints)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  // Every instruction does: NOT_STARTED -> NOT_FINISHED -> SUCCESS -> NOT_STARTED
  // Every breakpoint update also causes this to be called:
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(9));
  // All variables get an initial update and two of them are overwritten:
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(5));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(AtLeast(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  job.SetBreakpoint(1);
  job.SetBreakpoint(2);
  job.Start();
  JobState paused_job_state = JobState::kPaused;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(paused_job_state, 1.0));
  job.RemoveBreakpoint(2);
  job.Start();
  JobState successful_job_state = JobState::kSucceeded;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(successful_job_state, 1.0));
}

TEST_F(LocalJobTest, MoveConstructor)
{
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  // Every instruction does: NOT_STARTED -> NOT_FINISHED -> SUCCESS -> NOT_STARTED:
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(6));
  // All variables get an initial update and two of them are overwritten:
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(5));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(AtLeast(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  LocalJob moved{std::move(job)};
  moved.Start();
  JobState successful_job_state = JobState::kSucceeded;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(successful_job_state, 1.0));
}

TEST_F(LocalJobTest, MoveAssignment)
{
  // This is called for each job:
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(2));
  // Every instruction does: NOT_STARTED -> NOT_FINISHED -> SUCCESS -> NOT_STARTED:
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(6));
  // All variables get an initial update (for each job) and two of them are overwritten:
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(8));
  EXPECT_CALL(m_test_job_info_io, NextInstructionsUpdated(_)).Times(AtLeast(1));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc_1 = sup::oac_tree::ParseProcedureString(procedure_string);
  auto proc_2 = sup::oac_tree::ParseProcedureString(procedure_string);
  ASSERT_NE(proc_1.get(), nullptr);
  ASSERT_NE(proc_2.get(), nullptr);
  LocalJob job_1{std::move(proc_1), m_test_job_info_io};
  LocalJob job_2{std::move(proc_2), m_test_job_info_io};
  job_2 = std::move(job_1);
  job_2.Start();
  JobState successful_job_state = JobState::kSucceeded;
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(successful_job_state, 1.0));
}

TestJobInfoIO::TestJobInfoIO()
  : m_job_state{JobState::kInitial}
  , m_mtx{}
  , m_cv{}
{}

TestJobInfoIO::~TestJobInfoIO() = default;

void TestJobInfoIO::JobStateUpdated(JobState state)
{
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    m_job_state = state;
  }
  m_cv.notify_one();
}

bool TestJobInfoIO::WaitForJobState(JobState state, double timeout_sec)
{
  auto pred = [this, state]() {
    return m_job_state == state;
  };
  sup::dto::int64 timeout_ns;
  if (!instruction_utils::ConvertToTimeoutNanoseconds(timeout_sec, timeout_ns))
  {
    return false;
  }
  std::unique_lock<std::mutex> lk{m_mtx};
  return m_cv.wait_for(lk, std::chrono::nanoseconds(timeout_ns), pred);
}
