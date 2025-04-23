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

#ifndef SUP_OAC_TREE_CHOICE_H_
#define SUP_OAC_TREE_CHOICE_H_

#include <sup/oac-tree/compound_instruction.h>

#include <sup/dto/anyvalue.h>

#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Executes a subset of children depending on a variable value
 *
 * @note The variable either needs to encode a single index for the child to be executed
 * (i.e. a scalar positive integer) or an array of such indices.
 */
class Choice : public CompoundInstruction
{
public:
  Choice();

  virtual ~Choice();

  static const std::string Type;

private:
  std::vector<Instruction*> m_instruction_list;

  bool m_instruction_list_initialized;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook(UserInterface& ui) override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  bool CreateInstructionList(UserInterface& ui, Workspace& ws);

  /**
   * @brief Calculate this instruction's status from the status of its selected child instructions.
   */
  ExecutionStatus CalculateCompoundStatus() const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_CHOICE_H_
