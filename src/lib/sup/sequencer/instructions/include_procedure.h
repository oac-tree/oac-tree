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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_INCLUDE_PROCEDURE_H_
#define SUP_SEQUENCER_INCLUDE_PROCEDURE_H_

#include <sup/sequencer/decorator_instruction.h>
#include <sup/sequencer/workspace.h>

#include <memory>

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator instruction that includes an external procedure (workspace and instruction tree).
 */
class IncludeProcedure : public DecoratorInstruction
{
public:
  IncludeProcedure();

  ~IncludeProcedure() override;

  static const std::string Type;

private:
  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  bool PostInitialiseVariables(const StringAttributeList& source_attributes) override;

  Workspace* m_workspace;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INCLUDE_PROCEDURE_H_
