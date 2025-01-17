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

#ifndef SUP_OAC_TREE_INCLUDE_PROCEDURE_H_
#define SUP_OAC_TREE_INCLUDE_PROCEDURE_H_

#include <sup/oac-tree/decorator_instruction.h>
#include <sup/oac-tree/workspace.h>

#include <memory>

namespace sup
{
namespace oac_tree
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

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INCLUDE_PROCEDURE_H_
