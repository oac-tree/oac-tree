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

#ifndef SUP_OAC_TREE_LISTEN_H_
#define SUP_OAC_TREE_LISTEN_H_

#include <sup/oac-tree/decorator_instruction.h>

#include <sup/oac-tree/workspace.h>

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

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
  using VarState = std::pair<sup::dto::AnyValue, bool>;
  bool m_force_success;
  bool m_blocking;
  std::map<std::string, VarState> m_var_cache;
  bool m_var_changed;
  std::mutex m_mtx;
  std::condition_variable m_cv;
  ScopeGuard m_cb_guard;

  bool InitHook(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Repeatedly execute the child instruction when specific variables are updated.
   * When forceSuccess is not set, the instruction only ends on the child's FAILURE and returns
   * FAILURE accordingly.
   * The instruction also immediately returns SUCCESS in the absence of a child instruction.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void HaltImpl() override;

  void ResetHook(UserInterface& ui) override;

  void InitVariableCache(const std::vector<std::string>& var_names);

  void UpdateCallback(const std::string& name, const sup::dto::AnyValue& val, bool connected);

  void RegisterCallbacks(Workspace* ws, std::vector<std::string> var_names);

  void ClearCallbacks();

  /**
   * @brief Calculate this instruction's status from the status of its child instruction.
   */
  ExecutionStatus CalculateStatus() const;
};

template <>
inline DocumentationEntry InstructionDocFor<Listen>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Decorator that executes its child instruction each time specific variables are updated" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_LISTEN_H_
