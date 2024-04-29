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

#include <sup/sequencer/runner.h>

#include "breakpoint_manager.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_tree.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <algorithm>
#include <chrono>
#include <thread>

namespace sup
{
namespace sequencer
{

Runner::Runner(UserInterface& ui)
  : m_proc{nullptr}
  , m_ui{ui}
  , m_cb_guard{}
  , m_tick_cb{}
  , m_breakpoint_manager{new BreakpointManager{}}
  , m_current_breakpoint_instructions{}
  , m_halt{false}
{}

Runner::~Runner() = default;

void Runner::SetProcedure(Procedure* procedure)
{
  m_proc = procedure;
  auto callback = [this](const std::string& name, const sup::dto::AnyValue& value, bool connected)
    {
      m_ui.VariableUpdated(name, value, connected);
    };
  m_cb_guard = GetCallbackGuard(*m_proc, this);
  m_proc->RegisterGenericCallback(callback, this);
  m_proc->Setup();
  m_breakpoint_manager.reset(new BreakpointManager(*m_proc));
}

void Runner::SetTickCallback(TickCallback cb)
{
  m_tick_cb = cb;
}

bool Runner::SetBreakpoint(const Instruction* instruction)
{
  return m_breakpoint_manager->SetBreakpoint(instruction);
}

bool Runner::RemoveBreakpoint(const Instruction* instruction)
{
  return m_breakpoint_manager->RemoveBreakpoint(instruction);
}

std::vector<Breakpoint> Runner::GetBreakpoints() const
{
  auto list = m_breakpoint_manager->GetBreakpoints();
  return std::vector<Breakpoint>{ list.begin(), list.end() };
}

void Runner::ExecuteProcedure()
{
  m_halt.store(false);
  if (!m_proc)
  {
    return;
  }
  while (!IsFinished() && !m_halt.load())
  {
    auto next_instructions = GetNextInstructions(*m_proc);
    if (!next_instructions.empty())
    {
      m_current_breakpoint_instructions = m_breakpoint_manager->HandleBreakpoints(next_instructions);
      if (!m_current_breakpoint_instructions.empty())
      {
        return;
      }
    }
    ExecuteSingle();
  }
}

void Runner::ExecuteSingle()
{
  if (m_proc)
  {
    m_proc->ExecuteSingle(m_ui);
    m_breakpoint_manager->ResetBreakpoints();
    if (m_tick_cb)
    {
      m_tick_cb(*m_proc);
    }
  }
}

std::vector<const Instruction*> Runner::GetCurrentBreakpointInstructions() const
{
  return m_current_breakpoint_instructions;
}

void Runner::Halt()
{
  m_halt.store(true);
  if (m_proc)
  {
    m_proc->Halt();
  }
}

void Runner::Pause()
{
  m_halt.store(true);
}

bool Runner::IsFinished() const
{
  if (!m_proc)
  {
    return true;
  }
  auto status = m_proc->GetStatus();
  if (status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE)
  {
    return true;
  }
  auto root_instr = m_proc->RootInstruction();
  if (root_instr == nullptr || root_instr->IsHaltRequested())
  {
    return true;
  }
  return false;
}

bool Runner::IsRunning() const
{
  if (!m_proc)
  {
    return false;
  }

  auto status = m_proc->GetStatus();
  return (status == ExecutionStatus::RUNNING);
}

bool Runner::InstructionPresent(const Instruction* instruction) const
{
  if (!m_proc || !instruction)
  {
    return false;
  }
  auto root_instr = m_proc->RootInstruction();
  if (!root_instr)
  {
    return false;
  }
  auto instructions = FlattenBFS(CreateFullInstructionTree(root_instr));
  auto found_it = std::find(instructions.begin(), instructions.end(), instruction);
  return found_it != instructions.end();
}

TimeoutWhenRunning::TimeoutWhenRunning(int ms)
  : m_timeout_ms{ms}
{}

TimeoutWhenRunning::~TimeoutWhenRunning() = default;

void TimeoutWhenRunning::operator()(const Procedure& proc) const
{
  if (proc.GetStatus() == ExecutionStatus::RUNNING && m_timeout_ms > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(m_timeout_ms));
  }
}

}  // namespace sequencer

}  // namespace sup
