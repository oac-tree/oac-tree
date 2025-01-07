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

#include <sup/sequencer/application_utils.h>
#include <sup/sequencer/async_runner.h>
#include <sup/sequencer/job_state_monitor.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>

#include <gtest/gtest.h>

#include <atomic>

const std::string kTestParallelProcedureBody{R"(
  <ParallelSequence>
    <Wait timeout="0.2"/>
    <Wait timeout="0.2"/>
  </ParallelSequence>
  <Workspace/>
)"};

const std::string kTestSequenceProcedureBody{R"(
  <Sequence>
    <Wait timeout="0.2"/>
    <Wait timeout="0.2"/>
  </Sequence>
  <Workspace/>
)"};

const std::string kTestStepProcedureBody{R"(
  <Sequence>
    <Wait timeout="0.1"/>
    <Wait timeout="0.1"/>
  </Sequence>
  <Workspace/>
)"};

const std::string kLongWaitProcedureBody{R"(
  <Wait timeout="60"/>
  <Workspace/>
)"};

const std::string kVarCopyProcedureBody{R"(
  <Sequence>
    <Copy inputVar="one" outputVar="var1"/>
    <Copy inputVar="one" outputVar="var2"/>
  </Sequence>
  <Workspace>
    <Local name="one" type='{"type":"uint32"}' value='1'/>
    <Local name="var1" type='{"type":"uint32"}' value='0'/>
    <Local name="var2" type='{"type":"uint32"}' value='0'/>
  </Workspace>
)"};

using namespace sup::sequencer;
using namespace sup::sequencer;

class TestJobStateMonitor : public utils::SimpleJobStateMonitor
{
public:
  TestJobStateMonitor()
    : utils::SimpleJobStateMonitor{}
    , m_breakpoint_updates{}
    , m_ticks{}
  {}
  ~TestJobStateMonitor() = default;

  void OnBreakpointChange(const Instruction*, bool) noexcept override
  {
    ++m_breakpoint_updates;
  }

  void OnProcedureTick(const Procedure& proc) override
  {
    ++m_ticks;
  }

  sup::dto::uint32 GetBreakpointUpdateCount() const { return m_breakpoint_updates; }
  sup::dto::uint32 GetTickCount() const { return m_ticks; }
private:
  sup::dto::uint32 m_breakpoint_updates;
  sup::dto::uint32 m_ticks;
};

class AsyncRunnerTest : public ::testing::Test
{
protected:
  AsyncRunnerTest() = default;
  virtual ~AsyncRunnerTest() = default;

  bool WaitForState(JobState state, double seconds = 1.0)
  {
    return m_monitor.WaitForState(state, seconds);
  }
  TestJobStateMonitor m_monitor;
};

TEST_F(AsyncRunnerTest, Constructed)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestParallelProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kRunning));
  EXPECT_TRUE(WaitForState(JobState::kSucceeded, 2));
}

TEST_F(AsyncRunnerTest, Halted)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestParallelProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kRunning));
  async_runner.Halt();
  EXPECT_TRUE(WaitForState(JobState::kHalted));
}

TEST_F(AsyncRunnerTest, PauseStep)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestSequenceProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  async_runner.Pause();
  EXPECT_TRUE(WaitForState(JobState::kPaused));
  async_runner.Step();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
  EXPECT_EQ(m_monitor.GetTickCount(), 2u);
}

TEST_F(AsyncRunnerTest, StepReset)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestStepProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Step();
  EXPECT_TRUE(WaitForState(JobState::kStepping));
  EXPECT_TRUE(WaitForState(JobState::kPaused));
  EXPECT_EQ(m_monitor.GetTickCount(), 1u);
  async_runner.Step();
  EXPECT_TRUE(WaitForState(JobState::kStepping));
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
  EXPECT_EQ(m_monitor.GetTickCount(), 2u);
  async_runner.Reset();
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
}

TEST_F(AsyncRunnerTest, ResetWithWorkspace)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kVarCopyProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
  async_runner.Reset();
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
}

TEST_F(AsyncRunnerTest, TickCallbacks)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestStepProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  EXPECT_EQ(m_monitor.GetTickCount(), 0u);
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
  EXPECT_EQ(m_monitor.GetTickCount(), 2u);
}

TEST_F(AsyncRunnerTest, HaltLongInstruction)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kLongWaitProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kRunning));
  async_runner.Halt();
  // In rare cases, the halt came too fast and the wait instruction will not report failure,
  // because it didn't even start.
  EXPECT_TRUE(WaitForState(JobState::kFailed) || WaitForState(JobState::kHalted));
}

TEST_F(AsyncRunnerTest, Breakpoints)
{
  // Setup procedure and async_runner
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestStepProcedureBody));
  AsyncRunner async_runner{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  auto root_instr = proc->RootInstruction();
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 0u);

  // Add breakpoint to root
  async_runner.SetBreakpoint(root_instr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 1u);
  // Adding breakpoint to non-existing instruction does nothing
  async_runner.SetBreakpoint(nullptr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 1u);
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kPaused));

  // Remove breakpoint
  async_runner.RemoveBreakpoint(root_instr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 2u);
  // Trying to remove non-existent breakpoint does nothing
  async_runner.RemoveBreakpoint(root_instr);
  async_runner.RemoveBreakpoint(nullptr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 2u);

  // Finish procedure
  async_runner.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
}
