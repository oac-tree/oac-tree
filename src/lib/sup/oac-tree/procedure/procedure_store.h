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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_PROCEDURE_STORE_H_
#define SUP_OAC_TREE_PROCEDURE_STORE_H_

#include <map>
#include <memory>
#include <string>

namespace sup
{
namespace oac_tree
{
class Instruction;
class Procedure;
class UserInterface;
class Workspace;

/**
 * @brief ProcedureStore manages a cache of loaded subprocedures.
 */
class ProcedureStore
{
public:
  ProcedureStore(Procedure* parent);
  ~ProcedureStore();

  /**
   * @brief Retrieve (and possibly load) procedure from cache.
   */
  Procedure& LoadProcedure(const std::string& filename) const;

  /**
   * @brief Reset all owned procedures.
   */
  void ResetProcedureWorkspaces(UserInterface& ui) const;

  /**
   * @brief Tear down all owned procedures.
   */
  void TearDownProcedures(UserInterface& ui) const;

private:
  Procedure* m_parent;

  // Cache for procedures loaded from files and to be used by include type instructions.
  mutable std::map<std::string, std::unique_ptr<Procedure>> m_procedure_cache;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_PROCEDURE_STORE_H_
