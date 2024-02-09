/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
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

#include <sup/sequencer/job_controller.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/log_severity.h>

namespace sup
{
namespace sequencer
{
using namespace sup::sequencer;

JobController::JobController(Procedure& proc, UserInterface& ui, std::function<void(JobState)> cb)
  : m_proc{proc}
  , m_ui{ui}
  , m_runner{m_ui}
  , m_state_cb{cb}
  , m_command_queue{}
  , m_command_handler{}
  , m_loop_future{}
  , m_keep_alive{true}
{
  // Procedure MUST already be setup (since it's instruction tree cached should already have been built)
  SetState(JobState::kInitial);
  Launch();
}

JobController::~JobController()
{
  Terminate();
}

void JobController::Start()
{
  m_command_queue.Push(JobCommand::kStart);
}

void JobController::Step()
{
  m_command_queue.Push(JobCommand::kStep);
}

void JobController::Pause()
{
  m_command_queue.Push(JobCommand::kPause);
}

void JobController::Reset()
{
  m_command_queue.Push(JobCommand::kReset);
}

void JobController::Halt()
{
  auto halt_func = [this](){
    m_runner.Halt();
  };
  // The halt command needs to be handled first (except when there's a terminate pending).
  m_command_queue.PriorityPush(JobCommand::kHalt, halt_func);
}

void JobController::Terminate()
{
  auto terminate_func = [this](){
    m_runner.Halt();
  };
  // The terminate command needs to be handled first.
  m_command_queue.PriorityPush(JobCommand::kTerminate, terminate_func);
  m_loop_future.wait();
  m_proc.Reset(m_ui);
}

void JobController::SetState(JobState state)
{
  if (m_state_cb)
  {
    m_state_cb(state);
  }
  switch (state)
  {
    case JobState::kInitial:
      m_command_handler = &JobController::HandleInitial;
      break;
    case JobState::kPaused:
      m_command_handler = &JobController::HandlePaused;
      break;
    case JobState::kStepping:
      // Nothing to do here. State will switch immediately to paused after step.
      break;
    case JobState::kRunning:
      m_command_handler = &JobController::HandleRunning;
      break;
    case JobState::kSucceeded:
    case JobState::kFailed:
    case JobState::kHalted:
      m_command_handler = &JobController::HandleFinished;
      break;
    default:
      break;
  }
}

void JobController::Launch()
{
  m_runner.SetProcedure(std::addressof(m_proc));
  m_loop_future = std::async(std::launch::async, &JobController::ExecutionLoop, this);
}

JobController::Action JobController::HandleCommand(JobCommand command)
{
  return (this->*m_command_handler)(command);
}

JobController::Action JobController::HandleInitial(JobCommand command)
{
  switch (command)
  {
    case JobCommand::kStart:
      SetState(JobState::kRunning);
      return Action::kRun;
    case JobCommand::kStep:
      SetState(JobState::kStepping);
      return Action::kStep;
    case JobCommand::kPause:
      break;
    case JobCommand::kReset:
      break;
    case JobCommand::kHalt:
      SetState(JobState::kHalted);
      break;
    case JobCommand::kTerminate:
      m_keep_alive.store(false);
      return Action::kExit;
    default:
      break;
  }
  return Action::kContinue;
}

JobController::Action JobController::HandleRunning(JobCommand command)
{
  switch (command)
  {
    case JobCommand::kStart:
      break;
    case JobCommand::kStep:
    case JobCommand::kPause:
      m_runner.Pause();
      SetState(JobState::kPaused);
      break;
    case JobCommand::kReset:
      break;
    case JobCommand::kHalt:
      SetState(JobState::kHalted);
      break;
    case JobCommand::kTerminate:
      m_keep_alive.store(false);
      return Action::kExit;
    default:
      break;
  }
  return Action::kContinue;
}

JobController::Action JobController::HandlePaused(JobCommand command)
{
  switch (command)
  {
    case JobCommand::kStart:
      SetState(JobState::kRunning);
      return Action::kRun;
    case JobCommand::kStep:
      SetState(JobState::kStepping);
      return Action::kStep;
    case JobCommand::kPause:
      break;
    case JobCommand::kReset:
      break;
    case JobCommand::kHalt:
      SetState(JobState::kHalted);
      break;
    case JobCommand::kTerminate:
      m_keep_alive.store(false);
      return Action::kExit;
    default:
      break;
  }
  return Action::kContinue;
}

JobController::Action JobController::HandleFinished(JobCommand command)
{
  switch (command)
  {
    case JobCommand::kStart:
      break;
    case JobCommand::kStep:
      break;
    case JobCommand::kPause:
      break;
    case JobCommand::kReset:
      m_proc.Reset(m_ui);
      SetState(JobState::kInitial);
      break;
    case JobCommand::kHalt:
      break;
    case JobCommand::kTerminate:
      m_keep_alive.store(false);
      return Action::kExit;
    default:
      break;
  }
  return Action::kContinue;
}

void JobController::ExecutionLoop()
{
  while (m_keep_alive)
  {
    auto command = m_command_queue.WaitForNextCommand();
    auto action = HandleCommand(command);
    switch (action)
    {
      case Action::kContinue:
        continue;
      case Action::kStep:
        StepProcedure();
        break;
      case Action::kRun:
        RunProcedure();
        break;
      case Action::kExit:
        return;
    }
  }
}

void JobController::RunProcedure()
{
  auto tick_callback = [this](const sup::sequencer::Procedure& proc){
    ProcessCommandsWhenRunning();
    TimeoutWhenRunning timeout{TickTimeoutMs(proc)};
    timeout(proc);
    return;
  };
  m_runner.SetTickCallback(tick_callback);
  m_runner.ExecuteProcedure();
}

void JobController::ProcessCommandsWhenRunning()
{
  if (SwitchStateOnFinished())
  {
    return;
  }
  JobCommand command = JobCommand::kStart;
  if (m_command_queue.TryPop(command))
  {
    auto action = HandleCommand(command);
    // Ignore action, all necessary actions are already taken by HandleCommand.
    (void)action;
  }
}

void JobController::StepProcedure()
{
  m_runner.SetTickCallback(EmptyTickCallback{});
  m_runner.ExecuteSingle();
  if (SwitchStateOnFinished())
  {
    return;
  }
  SetState(JobState::kPaused);
}

bool JobController::SwitchStateOnFinished()
{
  auto status = m_proc.GetStatus();
  if (m_runner.IsFinished() && IsFinishedStatus(status))
  {
    if (m_proc.GetStatus() == ExecutionStatus::SUCCESS)
    {
      SetState(JobState::kSucceeded);
    }
    else
    {
      SetState(JobState::kFailed);
    }
    return true;
  }
  return false;
}

EmptyTickCallback::EmptyTickCallback() = default;

EmptyTickCallback::~EmptyTickCallback() = default;

void EmptyTickCallback::operator()(const sup::sequencer::Procedure&) const
{}

}  // namespace sequencer

}  // namespace sup
