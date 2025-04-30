/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
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

#include <sup/oac-tree/async_runner.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/log_severity.h>

namespace sup
{
namespace oac_tree
{
using namespace sup::oac_tree;

AsyncRunner::AsyncRunner(Procedure& proc, UserInterface& ui, JobStateMonitor& state_monitor)
  : m_proc{proc}
  , m_ui{ui}
  , m_runner{m_ui}
  , m_state_monitor{state_monitor}
  , m_command_queue{}
  , m_command_handler{}
  , m_loop_future{}
  , m_keep_alive{true}
{
  SetState(JobState::kInitial);
  Launch();
}

AsyncRunner::AsyncRunner(Procedure& proc, JobInterface& job_ui)
  : AsyncRunner{proc, job_ui, job_ui}
{}

AsyncRunner::~AsyncRunner()
{
  Terminate();
}

void AsyncRunner::SetBreakpoint(const Instruction* instruction)
{
  try
  {
    if (m_runner.SetBreakpoint(instruction))
    {
      m_state_monitor.OnBreakpointChange(instruction, true);
    }
  }
  catch(const MessageException&)
  {
    // Ignore.
  }
}

void AsyncRunner::RemoveBreakpoint(const Instruction* instruction)
{
  if (m_runner.RemoveBreakpoint(instruction))
  {
    m_state_monitor.OnBreakpointChange(instruction, false);
  }
}

void AsyncRunner::Start()
{
  m_command_queue.Push(JobCommand::kStart);
}

void AsyncRunner::Step()
{
  m_command_queue.Push(JobCommand::kStep);
}

void AsyncRunner::Pause()
{
  m_command_queue.Push(JobCommand::kPause);
}

void AsyncRunner::Reset()
{
  m_command_queue.Push(JobCommand::kReset);
}

void AsyncRunner::Halt()
{
  auto halt_func = [this](){
    m_runner.Halt();
  };
  // The halt command needs to be handled first (except when there's a terminate pending).
  m_command_queue.PriorityPush(JobCommand::kHalt, halt_func);
}

void AsyncRunner::Terminate()
{
  auto terminate_func = [this](){
    m_runner.Halt();
  };
  // The terminate command needs to be handled first.
  m_command_queue.PriorityPush(JobCommand::kTerminate, terminate_func);
  m_loop_future.wait();
}

void AsyncRunner::SetState(JobState state)
{
  m_state_monitor.OnStateChange(state);
  switch (state)
  {
    case JobState::kInitial:
      m_command_handler = &AsyncRunner::HandleInitial;
      break;
    case JobState::kPaused:
      m_command_handler = &AsyncRunner::HandlePaused;
      break;
    case JobState::kStepping:
      // Nothing to do here. State will switch immediately to paused after step.
      break;
    case JobState::kRunning:
      m_command_handler = &AsyncRunner::HandleRunning;
      break;
    case JobState::kSucceeded:
    case JobState::kFailed:
    case JobState::kHalted:
      m_command_handler = &AsyncRunner::HandleFinished;
      break;
    default:
      break;
  }
}

void AsyncRunner::Launch()
{
  m_runner.SetProcedure(std::addressof(m_proc));
  auto root_instr = m_proc.RootInstruction();
  m_loop_future = std::async(std::launch::async, &AsyncRunner::ExecutionLoop, this);
}

AsyncRunner::Action AsyncRunner::HandleCommand(JobCommand command)
{
  return (this->*m_command_handler)(command);
}

AsyncRunner::Action AsyncRunner::HandleInitial(JobCommand command)
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

AsyncRunner::Action AsyncRunner::HandleRunning(JobCommand command)
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

AsyncRunner::Action AsyncRunner::HandlePaused(JobCommand command)
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

AsyncRunner::Action AsyncRunner::HandleFinished(JobCommand command)
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

void AsyncRunner::ExecutionLoop()
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

void AsyncRunner::RunProcedure()
{
  const TimeoutWhenRunning timeout{TickTimeoutNs(m_proc)};
  auto tick_callback = [this, &timeout](const Procedure& proc){
    ProcessCommandsWhenRunning();
    timeout(proc);
    m_state_monitor.OnProcedureTick(proc);
    return;
  };
  m_runner.SetTickCallback(tick_callback);
  m_runner.ExecuteProcedure();
  // If a breakpoint was hit, set state to paused before handling other commands in the queue:
  if (m_runner.GetCurrentBreakpointInstructions().size() != 0)
  {
    SetState(JobState::kPaused);
  }
}

void AsyncRunner::ProcessCommandsWhenRunning()
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

void AsyncRunner::StepProcedure()
{
  auto tick_callback = [this](const Procedure& proc){
    m_state_monitor.OnProcedureTick(proc);
    return;
  };
  m_runner.SetTickCallback(tick_callback);
  m_runner.ExecuteSingle();
  if (!SwitchStateOnFinished())
  {
    SetState(JobState::kPaused);
  }
}

bool AsyncRunner::SwitchStateOnFinished()
{
  auto status = m_proc.GetStatus();
  if (IsFinishedStatus(status))
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

}  // namespace oac_tree

}  // namespace sup
