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

#ifndef SUP_OAC_TREE_COPY_TO_PROCEDURE_H_
#define SUP_OAC_TREE_COPY_TO_PROCEDURE_H_

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction that copies a variable from the current workspace into a workspace of an
 * external procedure. When including (parts of) that external procedure later, it will have access
 * to the copied value.
 *
 * @sa IncludeProcedure
 * @sa CopyFromProcedureInstruction
 */
class CopyToProcedureInstruction : public Instruction
{
public:
  CopyToProcedureInstruction();

  ~CopyToProcedureInstruction() override;

  static const std::string Type;

private:
  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  Workspace* m_workspace;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_COPY_TO_PROCEDURE_H_
