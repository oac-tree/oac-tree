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

#include <sup/sequencer/application_utils.h>
#include <sup/sequencer/job_controller.h>
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

  void OnProcedureTick(const Procedure& proc) noexcept override
  {
    ++m_ticks;
  }

  sup::dto::uint32 GetBreakpointUpdateCount() const { return m_breakpoint_updates; }
  sup::dto::uint32 GetTickCount() const { return m_ticks; }
private:
  sup::dto::uint32 m_breakpoint_updates;
  sup::dto::uint32 m_ticks;
};

class JobControllerTest : public ::testing::Test
{
protected:
  JobControllerTest() = default;
  virtual ~JobControllerTest() = default;

  bool WaitForState(JobState state, double seconds = 1.0)
  {
    return m_monitor.WaitForState(state, seconds);
  }
  TestJobStateMonitor m_monitor;
};

TEST_F(JobControllerTest, Constructed)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestParallelProcedureBody));
  JobController controller{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  controller.Start();
  EXPECT_TRUE(WaitForState(JobState::kRunning));
  EXPECT_TRUE(WaitForState(JobState::kSucceeded, 2));
}

TEST_F(JobControllerTest, Halted)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestParallelProcedureBody));
  JobController controller{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  controller.Start();
  EXPECT_TRUE(WaitForState(JobState::kRunning));
  controller.Halt();
  EXPECT_TRUE(WaitForState(JobState::kHalted));
}

TEST_F(JobControllerTest, PauseStep)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestSequenceProcedureBody));
  JobController controller{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  controller.Start();
  controller.Pause();
  EXPECT_TRUE(WaitForState(JobState::kPaused));
  controller.Step();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
}

TEST_F(JobControllerTest, StepReset)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestStepProcedureBody));
  JobController controller{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  controller.Step();
  EXPECT_TRUE(WaitForState(JobState::kStepping));
  EXPECT_TRUE(WaitForState(JobState::kPaused));
  controller.Step();
  EXPECT_TRUE(WaitForState(JobState::kStepping));
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
  controller.Reset();
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  controller.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
}

TEST_F(JobControllerTest, TickCallbacks)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestStepProcedureBody));
  JobController controller{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  EXPECT_EQ(m_monitor.GetTickCount(), 0u);
  controller.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
  EXPECT_EQ(m_monitor.GetTickCount(), 2u);
}

TEST_F(JobControllerTest, Breakpoints)
{
  // Setup procedure and controller
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestStepProcedureBody));
  JobController controller{*proc, ui, m_monitor};
  EXPECT_TRUE(WaitForState(JobState::kInitial));
  auto root_instr = proc->RootInstruction();
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 0u);

  // Add breakpoint to root
  controller.SetBreakpoint(root_instr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 1u);
  // Adding breakpoint to non-existing instruction does nothing
  controller.SetBreakpoint(nullptr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 1u);
  controller.Start();
  EXPECT_TRUE(WaitForState(JobState::kPaused));

  // Remove breakpoint
  controller.RemoveBreakpoint(root_instr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 2u);
  // Trying to remove non-existent breakpoint does nothing
  controller.RemoveBreakpoint(root_instr);
  controller.RemoveBreakpoint(nullptr);
  EXPECT_EQ(m_monitor.GetBreakpointUpdateCount(), 2u);

  // Finish procedure
  controller.Start();
  EXPECT_TRUE(WaitForState(JobState::kSucceeded));
}
