/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_PROCEDURE_STORE_H_
#define SUP_SEQUENCER_PROCEDURE_STORE_H_

#include <map>
#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
class Instruction;
class Procedure;
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
   * @brief Get procedure reference to a loaded procedure.
   *
   * @param filename Filename of the procedure to load or fetch from the cache.
   *
   * @return Reference to procedure.
   *
   * @details The ProcedureStore keeps a cache of loaded procedures to prevent loading the same
   * procedure file multiple times.
   */
  const Procedure& GetProcedure(const std::string& filename) const;

  /**
   * @brief Get workspace from loaded procedure.
   *
   * @param filename Filename of the procedure to load or fetch from the cache.
   *
   * @return Pointer to workspace.
   *
   * @note The returned workspace may already have been setup, but it's the client's responsibility
   * to ensure that is the case (calling Setup() twice on a workspace is null operation, so it's
   * safe to do that to be sure).
   */
  Workspace* GetWorkspace(const std::string& filename) const;

  /**
   * @brief Reset all owned procedures.
   */
  void ResetProcedures() const;

private:
  Procedure* m_parent;

  // Cache for procedures loaded from files and to be used by include type instructions.
  mutable std::map<std::string, std::unique_ptr<Procedure>> m_procedure_cache;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_STORE_H_
