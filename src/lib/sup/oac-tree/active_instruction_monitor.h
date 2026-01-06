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

#ifndef SUP_OAC_TREE_ACTIVE_INSTRUCTION_MONITOR_H_
#define SUP_OAC_TREE_ACTIVE_INSTRUCTION_MONITOR_H_

#include <sup/oac-tree/execution_status.h>

#include <sup/dto/basic_scalar_types.h>

#include <functional>
#include <set>

namespace sup
{
namespace oac_tree
{
/**
 * @brief This class monitors changes in the execution status of instructions to deduce the
 * currently active, i.e. executing, instructions.
 */
class ActiveInstructionMonitor
{
public:
  using ActiveInstructionsChanged = std::function<void(const std::set<sup::dto::uint32>&)>;
  using ActiveInstructionFilter =
    std::function<std::set<sup::dto::uint32>(const std::set<sup::dto::uint32>&)>;

  ActiveInstructionMonitor(ActiveInstructionsChanged callback, ActiveInstructionFilter filter);
  explicit ActiveInstructionMonitor(ActiveInstructionsChanged callback);
  ActiveInstructionMonitor(const ActiveInstructionMonitor &) = delete;
  ActiveInstructionMonitor(ActiveInstructionMonitor &&) = delete;
  ActiveInstructionMonitor &operator=(const ActiveInstructionMonitor &) = delete;
  ActiveInstructionMonitor &operator=(ActiveInstructionMonitor &&) = delete;
  ~ActiveInstructionMonitor();

  /**
   * @brief Notify an update of an instruction's execution status.
   *
   * @param instr_idx Index of the instruction.
   * @param status Updated execution status of the instruction.
   */
  void InstructionStatusUpdated(sup::dto::uint32 instr_idx, ExecutionStatus status);

private:
  ActiveInstructionsChanged m_callback;
  ActiveInstructionFilter m_filter;
  std::set<sup::dto::uint32> m_active_instructions; // List of currently active instruction indices

  bool AddActiveInstruction(sup::dto::uint32 instr_idx);
  bool RemoveActiveInstruction(sup::dto::uint32 instr_idx);

  void NotifyCallback() const;
};

/**
 * @brief Filter that returns the input set unchanged.
 *
 * @param instr_ids Set of instruction indices.
 * @return Copy of the input set.
 */
std::set<sup::dto::uint32> IdentityFilter(const std::set<sup::dto::uint32>& instr_ids);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ACTIVE_INSTRUCTION_MONITOR_H_
