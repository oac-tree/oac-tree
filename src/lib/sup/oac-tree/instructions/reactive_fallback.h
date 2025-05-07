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

#ifndef SUP_OAC_TREE_REACTIVE_FALLBACK_H_
#define SUP_OAC_TREE_REACTIVE_FALLBACK_H_

#include <sup/oac-tree/compound_instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Compound instruction representing a reactive fallback.
 *
 * @note Fallback keeps on executing its children until one returns successfully. It fails when all
 * child instructions fail. If a child returns RUNNING, all other child instructions are reset so
 * they might be executed next time.
 */
class ReactiveFallback : public CompoundInstruction
{
public:
  ReactiveFallback();

  ~ReactiveFallback() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  /**
   * @brief Calculate this instruction's status from the status of its child instructions.
   */
  ExecutionStatus CalculateCompoundStatus() const;

  /**
   * @brief Reset all other child instructions.
   *
   * @param instr The instruction that is running and should not be reset.
   * @param ui UserInterface to handle status updates.
   */
  void ResetOtherChildren(const Instruction* instr, UserInterface& ui);
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_REACTIVE_FALLBACK_H_
