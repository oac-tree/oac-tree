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

#ifndef SUP_OAC_TREE_FORCE_SUCCESS_H_
#define SUP_OAC_TREE_FORCE_SUCCESS_H_

#include <sup/oac-tree/decorator_instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Wraps a child instruction and always return SUCCESS when the child has finished
 * execution.
 */
class ForceSuccess : public DecoratorInstruction
{
public:
  ForceSuccess();

  ~ForceSuccess() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
};

}  // namespace oac_tree

}  // namespace sup

#endif /* SUP_OAC_TREE_FORCE_SUCCESS_H_ */
