/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#ifndef SUP_OAC_TREE_FOR_H_
#define SUP_OAC_TREE_FOR_H_

#include <sup/oac-tree/decorator_instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction decorator that that applies a child instruction to the elements of an array
 *
 * @details Executes the child instruction on the elements of an array, until either: the child
 * fails (FAILURE) or the child instruction is applied to all elements of the provided array
 * (SUCCESS).
 */
class ForInstruction : public DecoratorInstruction
{
public:
  ForInstruction();

  ~ForInstruction() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction
   * and the number of successful executions of the child instruction.
   */
  ExecutionStatus CalculateStatus(int max_count) const;

  int m_count;
  sup::dto::AnyValue m_array;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_FOR_H_
