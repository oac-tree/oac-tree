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

#include <sup/sequencer/constants.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <algorithm>
#include <chrono>
#include <thread>

namespace
{
using sup::sequencer::Breakpoint;
using sup::sequencer::Instruction;
std::vector<Breakpoint>::const_iterator FindBreakpoint(const std::vector<Breakpoint>& breakpoints,
                                                       const Instruction* instruction);
std::vector<Breakpoint>::iterator FindBreakpoint(std::vector<Breakpoint>& breakpoints,
                                                 const Instruction* instruction);
}

namespace sup
{
namespace sequencer
{
static int TickTimeoutMs(Procedure& procedure);

std::vector<Breakpoint*> CurrentBreakpoints(
  std::vector<Breakpoint>& breakpoints, const std::vector<const Instruction*>& next_instructions);

std::vector<const Instruction*> HandleBreakpoints(
  std::vector<Breakpoint>& breakpoints, const std::vector<const Instruction*>& next_instructions);

void ResetBreakpoints(std::vector<Breakpoint>& breakpoints,
                      const std::vector<const Instruction*>& current_breakpoint_instructions);

Runner::Runner(UserInterface& ui)
  : m_proc{nullptr}
  , m_ui{ui}
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

void Runner::SetBreakpoint(const Instruction* instruction)
{
  auto it = FindBreakpoint(m_breakpoints, instruction);
  if (it == m_breakpoints.end())
  {
    m_breakpoints.emplace_back(instruction);
  }
}

void Runner::ExecuteProcedure()
{
  m_halt.store(false);
  ResetBreakpoints(m_breakpoints, m_current_breakpoint_instructions);
  if (m_proc)
  {
    auto sleep_time_ms = TickTimeoutMs(*m_proc);

    while (!IsFinished() && !m_halt.load())
    {
      auto next_instructions = m_proc->GetNextInstructions();
      m_current_breakpoint_instructions = HandleBreakpoints(m_breakpoints, next_instructions);
      if (!m_current_breakpoint_instructions.empty())
      {
        return;
      }
      ExecuteSingle();
      if (IsRunning())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
      }
    }
  }
}

void Runner::ExecuteSingle()
{
  if (m_proc)
  {
    m_ui.StartSingleStep();
    m_proc->ExecuteSingle(m_ui);
    m_ui.EndSingleStep();
  }
}

void Runner::Halt()
{
  m_halt.store(true);
  if (m_proc)
  {
    m_proc->Halt();
  }
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

static int TickTimeoutMs(Procedure& procedure)
{
  if (procedure.HasAttribute(kTickTimeoutAttributeName))
  {
    auto tick_timeout = procedure.GetAttributeValue<double>(kTickTimeoutAttributeName);
    if (tick_timeout > 0.001)
    {
      return static_cast<int>(tick_timeout * 1000);
    }
  }
  return DefaultSettings::DEFAULT_SLEEP_TIME_MS;
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

void ResetBreakpoints(std::vector<Breakpoint>& breakpoints,
                      const std::vector<const Instruction*>& current_breakpoint_instructions)
{
  for (auto current_breakpoint_instruction : current_breakpoint_instructions)
  {
    auto it = FindBreakpoint(breakpoints, current_breakpoint_instruction);
    if (it != breakpoints.end() && it->GetStatus() == Breakpoint::kReleased)
    {
      it->SetStatus(Breakpoint::kSet);
    }
  }
}

}  // namespace sequencer

}  // namespace sup

namespace
{
std::vector<Breakpoint>::const_iterator FindBreakpoint(const std::vector<Breakpoint>& breakpoints,
                                                       const Instruction* instruction)
{
  auto predicate = [instruction](const Breakpoint& breakpoint) {
    return breakpoint.GetInstruction() == instruction;
  };
  return std::find_if(breakpoints.begin(), breakpoints.end(), predicate);
}

std::vector<Breakpoint>::iterator FindBreakpoint(std::vector<Breakpoint>& breakpoints,
                                                 const Instruction* instruction)
{
  auto predicate = [instruction](Breakpoint& breakpoint) {
    return breakpoint.GetInstruction() == instruction;
  };
  return std::find_if(breakpoints.begin(), breakpoints.end(), predicate);
}

}
