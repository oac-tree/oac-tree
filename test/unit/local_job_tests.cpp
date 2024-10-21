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
#include <sup/sequencer/instruction_utils.h>
#include <sup/sequencer/local_job.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <mutex>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Exactly;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::SetArgReferee;

using namespace sup::sequencer;

class TestJobInfoIO : public sup::sequencer::IJobInfoIO
{
public:
  TestJobInfoIO();
  ~TestJobInfoIO() override;
  MOCK_METHOD(void, InitNumberOfInstructions, (sup::dto::uint32), (override));
  MOCK_METHOD(void, InstructionStateUpdated, (sup::dto::uint32, sup::sequencer::InstructionState), (override));
  MOCK_METHOD(void, VariableUpdated, (sup::dto::uint32, const sup::dto::AnyValue&, bool), (override));
  MOCK_METHOD(void, PutValue, (const sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(bool, GetUserValue, (sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(int, GetUserChoice, (const std::vector<std::string>&, const sup::dto::AnyValue&), (override));
  MOCK_METHOD(void, Message, (const std::string&), (override));
  MOCK_METHOD(void, Log, (int, const std::string&), (override));
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

  TestJobInfoIO m_test_job_info_io;
};

TEST_F(LocalJobTest, Construction)
{
  JobState initial_job_state = JobState::kInitial;
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(3));
  EXPECT_CALL(m_test_job_info_io, PutValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserChoice(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Message(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Log(_, _)).Times(Exactly(0));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  EXPECT_TRUE(m_test_job_info_io.WaitForJobState(initial_job_state, 0.0));
}

TEST_F(LocalJobTest, Start)
{
  JobState successful_job_state = JobState::kSucceeded;
  EXPECT_CALL(m_test_job_info_io, InitNumberOfInstructions(3)).Times(Exactly(1));
  // Every instruction does: NOT_STARTED -> NOT_FINISHED -> SUCCESS -> NOT_STARTED:
  EXPECT_CALL(m_test_job_info_io, InstructionStateUpdated(_, _)).Times(Exactly(9));
  EXPECT_CALL(m_test_job_info_io, VariableUpdated(_, _, true)).Times(Exactly(5));
  EXPECT_CALL(m_test_job_info_io, PutValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserValue(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, GetUserChoice(_, _)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Message(_)).Times(Exactly(0));
  EXPECT_CALL(m_test_job_info_io, Log(_, _)).Times(Exactly(0));

  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  LocalJob job{std::move(proc), m_test_job_info_io};
  job.Start();
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
