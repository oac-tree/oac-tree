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

#ifndef SUP_OAC_TREE_SEQUENCE_H_
#define SUP_OAC_TREE_SEQUENCE_H_

#include <sup/oac-tree/compound_instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Compound instruction representing a sequence.
 *
 * @note Sequence keeps executing its children as long as they return SUCCESS. Sequence fails as
 * soon as a child fails.
 */
class Sequence : public CompoundInstruction
{
public:
  Sequence();

  ~Sequence() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  /**
   * @brief Calculate this instruction's status from the status of its child instructions.
   */
  ExecutionStatus CalculateCompoundStatus() const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_SEQUENCE_H_
