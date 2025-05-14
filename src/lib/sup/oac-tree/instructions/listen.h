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

#ifndef SUP_OAC_TREE_LISTEN_H_
#define SUP_OAC_TREE_LISTEN_H_

#include <sup/oac-tree/decorator_instruction.h>

#include <sup/oac-tree/workspace.h>

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Decorator that executes its child instruction each time specific variables are updated.
 */
class Listen : public DecoratorInstruction
{
public:
  Listen();

  ~Listen() override;

  static const std::string Type;

private:
  bool m_force_success;
  std::map<std::string, sup::dto::AnyValue> m_var_cache;

  bool InitHook(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Repeatedly execute the child instruction when specific variables are updated.
   * When forceSuccess is not set, the instruction only ends on the child's FAILURE and returns
   * FAILURE accordingly.
   * The instruction also immediately returns SUCCESS in the absence of a child instruction.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook(UserInterface& ui) override;

  void InitVariableCache(const std::vector<std::string>& var_names);

  bool UpdateVariableCache(Workspace& ws);

  /**
   * @brief Calculate this instruction's status from the status of its child instruction.
   */
  ExecutionStatus CalculateStatus() const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_LISTEN_H_
