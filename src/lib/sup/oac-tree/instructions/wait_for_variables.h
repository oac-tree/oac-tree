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

#ifndef SUP_OAC_TREE_WAIT_FOR_VARIABLES_H_
#define SUP_OAC_TREE_WAIT_FOR_VARIABLES_H_

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/scope_guard.h>

#include <sup/dto/basic_scalar_types.h>

#include <condition_variable>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction that waits 'timeout' seconds for all variables of a certain type to be
 * available, i.e. as defined by Variable::IsAvailable().
 *
 * @details The 'timeout' attribute is mandatory.
 */
class WaitForVariables : public Instruction
{
public:
  WaitForVariables();

  ~WaitForVariables() override;

  static const std::string Type;

private:
  bool m_blocking;
  sup::dto::int64 m_finish;
  std::vector<std::string> m_var_names;
  mutable std::vector<std::string> m_unavailable_vars;
  bool m_updated;
  std::mutex m_mtx;
  std::condition_variable m_cv;
  ScopeGuard m_cb_guard;

  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void HaltImpl() override;

  void ResetHook(UserInterface& ui) override;

  ExecutionStatus ExecuteBlocking(UserInterface& ui, Workspace& ws);

  void RegisterCallbacks(Workspace& ws);

  void ClearCallbacks();

  bool CheckUpdate();

  ExecutionStatus HandleFinishedStatus(ExecutionStatus status);

  bool CheckCondition(Workspace& ws) const;

  void LogUnavailableVars(UserInterface& ui) const;
};

template <>
inline DocumentationEntry InstructionDocFor<WaitForVariables>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction node that waits 'timeout' seconds for all variables of a certain type to be available" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_WAIT_FOR_VARIABLES_H_
