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

#ifndef SUP_OAC_TREE_REPEAT_H_
#define SUP_OAC_TREE_REPEAT_H_

#include <sup/oac-tree/decorator_instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction decorator that repeats its child a fixed number of times while successful.
 *
 * @details Repeatedly executes the child instruction, until either: the child fails (FAILURE)
 * or maximum number of repetitions is reached (SUCCESS).
 */
class Repeat : public DecoratorInstruction
{
public:
  Repeat();

  ~Repeat() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction
   * and the number of successful executions of the child instruction.
   */
  ExecutionStatus CalculateStatus() const;

  sup::dto::int32 m_max_count;
  sup::dto::int32 m_count;
  bool m_init_ok;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_REPEAT_H_
