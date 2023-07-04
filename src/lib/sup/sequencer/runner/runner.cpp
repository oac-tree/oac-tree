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
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_tree.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <algorithm>
#include <chrono>
#include <thread>

namespace
{
using sup::sequencer::Breakpoint;
using sup::sequencer::Instruction;
std::vector<Breakpoint>::iterator FindBreakpoint(std::vector<Breakpoint>& breakpoints,
                                                 const Instruction* instruction);
std::vector<Breakpoint*> CurrentBreakpoints(
  std::vector<Breakpoint>& breakpoints, const std::vector<const Instruction*>& next_instructions);

std::vector<const Instruction*> HandleBreakpoints(
  std::vector<Breakpoint>& breakpoints, const std::vector<const Instruction*>& next_instructions);

void ResetBreakpoints(std::vector<Breakpoint>& breakpoints);
}

namespace sup
{
namespace sequencer
{

Runner::Runner(UserInterface& ui)
  : m_proc{nullptr}
  , m_ui{ui}
  , m_tick_cb{}
  , m_breakpoints{}
  , m_current_breakpoint_instructions{}
  , m_halt{false}
{}

Runner::~Runner() = default;

void Runner::SetProcedure(Procedure* procedure)
{
  m_proc = procedure;
  m_proc->RegisterGenericCallback(
    [this](const std::string& name, const sup::dto::AnyValue& value, bool connected)
    {
      m_ui.VariableUpdated(name, value, connected);
    });
}

void Runner::SetTickCallback(TickCallback cb)
{
  m_tick_cb = cb;
}

void Runner::SetBreakpoint(const Instruction* instruction)
{
  if (!InstructionPresent(instruction))
  {
    std::string error_message =
      "Runner::SetBreakpoint: trying to set a breakpoint at non-existent instruction";
    throw InvalidOperationException(error_message);
  }
  auto it = FindBreakpoint(m_breakpoints, instruction);
  if (it == m_breakpoints.end())
  {
    m_breakpoints.emplace_back(instruction);
  }
}

void Runner::RemoveBreakpoint(const Instruction* instruction)
{
  auto it = FindBreakpoint(m_breakpoints, instruction);
  if (it == m_breakpoints.end())
  {
    return;
  }
  m_breakpoints.erase(it);
}

void Runner::DisableBreakpoint(const Instruction* instruction)
{
  auto it = FindBreakpoint(m_breakpoints, instruction);
  if (it == m_breakpoints.end())
  {
    return;
  }
  it->SetStatus(Breakpoint::kDisabled);
}

void Runner::EnableBreakpoint(const Instruction* instruction)
{
  auto it = FindBreakpoint(m_breakpoints, instruction);
  if (it == m_breakpoints.end())
  {
    return;
  }
  it->SetStatus(Breakpoint::kSet);
}

std::vector<Breakpoint> Runner::GetBreakpoints() const
{
  return m_breakpoints;
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
      m_current_breakpoint_instructions = HandleBreakpoints(m_breakpoints, next_instructions);
      if (!m_current_breakpoint_instructions.empty())
      {
        return;
      }
    }
    ExecuteSingle();
    ResetBreakpoints(m_breakpoints);
    if (m_tick_cb)
    {
      m_tick_cb(*m_proc);
    }
  }
}

void Runner::ExecuteSingle()
{
  if (m_proc)
  {
    m_proc->ExecuteSingle(m_ui);
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
  return (status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE);
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

namespace
{
std::vector<Breakpoint>::iterator FindBreakpoint(std::vector<Breakpoint>& breakpoints,
                                                 const Instruction* instruction)
{
  auto predicate = [instruction](Breakpoint& breakpoint) {
    return breakpoint.GetInstruction() == instruction;
  };
  return std::find_if(breakpoints.begin(), breakpoints.end(), predicate);
}

std::vector<Breakpoint*> CurrentBreakpoints(
  std::vector<Breakpoint>& breakpoints, const std::vector<const Instruction*>& next_instructions)
{
  std::vector<Breakpoint*> current_breakpoints;
  for (auto& breakpoint : breakpoints)
  {
    auto status = breakpoint.GetStatus();
    if (status == Breakpoint::kDisabled)
    {
      continue;
    }
    if (std::find(next_instructions.begin(), next_instructions.end(), breakpoint.GetInstruction())
        == next_instructions.end())
    {
      continue;
    }
    current_breakpoints.push_back(&breakpoint);
  }
  return current_breakpoints;
}

std::vector<const Instruction*> HandleBreakpoints(
  std::vector<Breakpoint>& breakpoints, const std::vector<const Instruction*>& next_instructions)
{
  std::vector<const Instruction*> result;
  auto current_breakpoints = CurrentBreakpoints(breakpoints, next_instructions);
  for (auto current_breakpoint : current_breakpoints)
  {
    auto status = current_breakpoint->GetStatus();
    if (status == Breakpoint::kSet)
    {
      result.push_back(current_breakpoint->GetInstruction());
      current_breakpoint->SetStatus(Breakpoint::kReleased);
    }
  }
  return result;
}

void ResetBreakpoints(std::vector<Breakpoint>& breakpoints)
{
  for (auto& breakpoint : breakpoints)
  {
    if (breakpoint.GetStatus() == Breakpoint::kReleased)
    {
      breakpoint.SetStatus(Breakpoint::kSet);
    }
  }
}
}
