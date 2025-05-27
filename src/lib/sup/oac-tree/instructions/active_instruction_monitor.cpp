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

#include <sup/oac-tree/active_instruction_monitor.h>

namespace sup
{
namespace oac_tree
{
ActiveInstructionMonitor::ActiveInstructionMonitor()
  : m_active_instructions{}
{}

ActiveInstructionMonitor::~ActiveInstructionMonitor() = default;

void ActiveInstructionMonitor::InstructionStatusUpdated(
  sup::dto::uint32 instr_idx, ExecutionStatus status)
{
  if (IsExecutingStatus(status))
  {
    (void)AddActiveInstruction(instr_idx);
  }
  else
  {
    (void)RemoveActiveInstruction(instr_idx);
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

}  // namespace oac_tree

}  // namespace sup
