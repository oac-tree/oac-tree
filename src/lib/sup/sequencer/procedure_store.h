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
 * @brief Procedure contains a tree of instructions
 *
 * @details A Procedure object contains a full instruction tree and a workspace
 * @note The client of the Procedure, i.e. the object responsible for its creation and
 * destruction, needs to ensure that the Procedure is correctly set up before executing it (by
 * calling the Setup method on it). Likewise, the client needs to call Reset on the Procedure
 * before destroying the UserInterface class. Reset will block until all threads have terminated.
 */
class ProcedureStore
{
public:
  ProcedureStore();
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
   * @brief Get (clone of) instruction tree from a loaded procedure.
   *
   * @param filename Filename of the procedure to load or fetch from the cache.
   * @param path Path of the root instruction to clone inside the procedure (empty for the root
   * instruction of the procedure).
   *
   * @return Instruction tree.
   */
  std::unique_ptr<Instruction> CloneInstructionFromProcedure(const std::string& filename,
                                                             const std::string& path) const;

  /**
   * @brief Get workspace from loaded procedure.
   *
   * @param filename Filename of the procedure to load or fetch from the cache.
   *
   * @return Workspace.
   */
  std::unique_ptr<Workspace> GetWorkspace(const std::string& filename) const;

  /**
   * @brief Clear the cached procedures.
   */
  void ClearProcedureCache() const;

private:
  // Cache for procedures loaded from files and to be used by include type instructions.
  mutable std::map<std::string, std::unique_ptr<Procedure>> m_procedure_cache;
};

std::unique_ptr<Instruction> CloneInstructionPath(const Procedure& proc, const std::string& path);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_STORE_H_
