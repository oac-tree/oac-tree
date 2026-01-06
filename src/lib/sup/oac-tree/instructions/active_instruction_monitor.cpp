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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <sup/oac-tree/active_instruction_monitor.h>

namespace sup
{
namespace oac_tree
{
ActiveInstructionMonitor::ActiveInstructionMonitor(ActiveInstructionsChanged callback,
                                                   ActiveInstructionFilter filter)
  : m_callback{std::move(callback)}
  , m_filter{std::move(filter)}
  , m_active_instructions{}
{}

ActiveInstructionMonitor::ActiveInstructionMonitor(ActiveInstructionsChanged callback)
  : ActiveInstructionMonitor(std::move(callback), IdentityFilter)
{}

ActiveInstructionMonitor::~ActiveInstructionMonitor() = default;

void ActiveInstructionMonitor::InstructionStatusUpdated(
  sup::dto::uint32 instr_idx, ExecutionStatus status)
{
  if (IsExecutingStatus(status))
  {
    if (AddActiveInstruction(instr_idx))
    {
      NotifyCallback();
    }
  }
  else
  {
    if (RemoveActiveInstruction(instr_idx))
    {
      NotifyCallback();
    }
  }
}

bool ActiveInstructionMonitor::AddActiveInstruction(sup::dto::uint32 instr_idx)
{
  auto result = m_active_instructions.insert(instr_idx);
  return result.second;
}

bool ActiveInstructionMonitor::RemoveActiveInstruction(sup::dto::uint32 instr_idx)
{
  return m_active_instructions.erase(instr_idx) > 0;
}

void ActiveInstructionMonitor::NotifyCallback() const
{
  m_callback(m_filter(m_active_instructions));
}

std::set<sup::dto::uint32> IdentityFilter(const std::set<sup::dto::uint32>& instr_ids)
{
  return instr_ids;
}

}  // namespace oac_tree

}  // namespace sup
