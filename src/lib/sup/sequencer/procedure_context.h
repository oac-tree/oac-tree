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

#ifndef SUP_SEQUENCER_PROCEDURE_CONTEXT_H_
#define SUP_SEQUENCER_PROCEDURE_CONTEXT_H_

#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
class Instruction;
class Procedure;
class ProcedureStore;
class Workspace;

/**
 * @brief Structure containing the context of a procedure.
 *
 */
class ProcedureContext
{
public:
  ProcedureContext(const std::string& filename, const ProcedureStore& procedure_store);
  ~ProcedureContext();

  /**
   * @brief Get filename of the current context.
   */
  std::string GetFilename() const;

  /**
   * @brief Get procedure reference to a loaded procedure.
   *
   * @param filename Filename of the procedure to fetch from the store.
   *
   * @return Reference to procedure.
   */
  const Procedure& GetProcedure(const std::string& filename) const;

  /**
   * @brief Get workspace from loaded procedure.
   *
   * @param filename Filename of the procedure to fetch from the store.
   *
   * @return Pointer to workspace.
   *
   * @note The returned workspace may already have been setup, but it's the client's responsibility
   * to ensure that is the case (calling Setup() twice on a workspace is null operation, so it's
   * safe to do that to be sure).
   */
  Workspace& GetWorkspace(const std::string& filename) const;

  /**
   * @brief Clone an instruction tree wth given path from a procedure with given name.
   *
   * @param filename Filename of the procedure to fetch from the store.
   * @param path Path to the root of the instruction tree to clone.
   *
   * @return Pointer to workspace.
   */
  std::unique_ptr<Instruction> CloneInstructionPath(const std::string& filename,
                                                    const std::string& path) const;

private:
  std::string m_procedure_filename;
  const ProcedureStore* m_procedure_store;
};

/**
 * @brief Clone an instruction tree with given root instruction.
 *
 * @param instruction Root of the tree to clone.
 * @return Cloned instruction tree.
 */
std::unique_ptr<Instruction> CloneInstructionTree(const Instruction& instruction);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_CONTEXT_H_
