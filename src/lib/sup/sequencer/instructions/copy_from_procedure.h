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

#ifndef SUP_SEQUENCER_COPY_FROM_PROCEDURE_H_
#define SUP_SEQUENCER_COPY_FROM_PROCEDURE_H_

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction that copies a variable from the workspace of an external procedure into the
 * current workspace.
 *
 * @sa IncludeProcedure
 * @sa CopyToProcedureInstruction
 */
class CopyFromProcedureInstruction : public Instruction
{
public:
  CopyFromProcedureInstruction();

  ~CopyFromProcedureInstruction() override;

  static const std::string Type;

private:
  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  Workspace* m_workspace;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_COPY_FROM_PROCEDURE_H_
