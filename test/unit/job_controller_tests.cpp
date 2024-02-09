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

#include <sup/sequencer/job_controller.h>
#include <sup/sequencer/job_state_monitor.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>

#include <gtest/gtest.h>

#include <atomic>

const std::string kTestParallelProcedureBody{R"(
  <ParallelSequence>
    <Wait timeout="1.0"/>
    <Wait timeout="1.0"/>
  </ParallelSequence>
  <Workspace/>
)"};

const std::string kTestSequenceProcedureBody{R"(
  <Sequence>
    <Wait timeout="0.5"/>
    <Wait timeout="0.5"/>
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

class JobControllerTest : public ::testing::Test
{
protected:
  JobControllerTest() = default;
  virtual ~JobControllerTest() = default;

  std::function<void(JobState)> GetStateCallback()
  {
    return m_monitor.GetStateCallback();
  }

  bool WaitForState(JobState state, double seconds = 1.0)
  {
    return m_monitor.WaitForState(state, seconds);
  }

  JobStateMonitor m_monitor;
};

TEST_F(JobControllerTest, Constructed)
{
  DefaultUserInterface ui;
  auto proc = ParseProcedureString(
    sup::UnitTestHelper::CreateProcedureString(kTestParallelProcedureBody));
  JobController controller{*proc, ui, GetStateCallback()};
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
  JobController controller{*proc, ui, GetStateCallback()};
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
  JobController controller{*proc, ui, GetStateCallback()};
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
  JobController controller{*proc, ui, GetStateCallback()};
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
