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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "breakpoint_manager.h"

#include <sup/oac-tree/instruction_tree.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/breakpoint.h>

#include <algorithm>

namespace
{
using namespace sup::oac_tree;
std::set<const Instruction*> GetInstructionList(const Procedure& proc);
}  // unnamed namespace

namespace sup
{
namespace oac_tree
{

BreakpointManager::BreakpointManager()
  : m_instruction_list{}
  , m_breakpoints{}
  , m_mtx{}
{}

BreakpointManager::BreakpointManager(const Procedure& proc)
  : m_instruction_list{GetInstructionList(proc)}
  , m_breakpoints{}
  , m_mtx{}
{}

BreakpointManager::~BreakpointManager() = default;

bool BreakpointManager::SetBreakpoint(const Instruction* instruction)
{
  if (!IsKnownInstruction(instruction))
  {
    std::string error_message =
      "BreakpointManager::SetBreakpoint: trying to set a breakpoint at non-existent instruction";
    throw InvalidOperationException(error_message);
  }
  auto predicate = [instruction](const Breakpoint& breakpoint) {
    return breakpoint.GetInstruction() == instruction;
  };
  std::lock_guard<std::mutex> lk{m_mtx};
  auto iter = std::find_if(m_breakpoints.begin(), m_breakpoints.end(), predicate);
  if (iter == m_breakpoints.end())
  {
    m_breakpoints.emplace_back(instruction);
    return true;
  }
  return false;
}

bool BreakpointManager::RemoveBreakpoint(const Instruction* instruction)
{
  auto predicate = [instruction](const Breakpoint& breakpoint) {
    return breakpoint.GetInstruction() == instruction;
  };
  std::lock_guard<std::mutex> lk{m_mtx};
  auto iter = std::find_if(m_breakpoints.begin(), m_breakpoints.end(), predicate);
  if (iter == m_breakpoints.end())
  {
    return false;
  }
  m_breakpoints.erase(iter);
  return true;
}

std::vector<const Instruction*> BreakpointManager::HandleBreakpoints(
  const std::vector<const Instruction*>& next_instructions)
{
  std::vector<const Instruction*> result;
  std::lock_guard<std::mutex> lk{m_mtx};
  for (auto& breakpoint : m_breakpoints)
  {
    if (std::find(next_instructions.begin(), next_instructions.end(), breakpoint.GetInstruction())
        == next_instructions.end())
    {
      continue;
    }
    auto status = breakpoint.GetStatus();
    if (status == Breakpoint::kSet)
    {
      result.push_back(breakpoint.GetInstruction());
      breakpoint.SetStatus(Breakpoint::kReleased);
    }
  }
  return result;
}

void BreakpointManager::ResetBreakpoints()
{
  std::lock_guard<std::mutex> lk{m_mtx};
  for (auto& breakpoint : m_breakpoints)
  {
    if (breakpoint.GetStatus() == Breakpoint::kReleased)
    {
      breakpoint.SetStatus(Breakpoint::kSet);
    }
  }
}

std::list<Breakpoint> BreakpointManager::GetBreakpoints() const
{
  std::lock_guard<std::mutex> lk{m_mtx};
  return m_breakpoints;
}

bool BreakpointManager::IsKnownInstruction(const Instruction* instruction) const
{
  auto iter = m_instruction_list.find(instruction);
  return iter != m_instruction_list.end();
}

}  // namespace oac_tree

}  // namespace sup

namespace
{
using namespace sup::oac_tree;
std::set<const Instruction*> GetInstructionList(const Procedure& proc)
{
  auto root_instr = proc.RootInstruction();
  if (!root_instr)
  {
    return {};
  }
  auto instructions = FlattenBFS(CreateFullInstructionTree(root_instr));
  return std::set<const Instruction*>{ instructions.begin(), instructions.end() };
}
}  // unnamed namespace

