/******************************************************************************
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Rui Oliveira (EXT)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef SUP_OAC_TREE_ADD_ELEMENT_H
#define SUP_OAC_TREE_ADD_ELEMENT_H

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction for adding a new element to a workspace array variable
 *
 * @details An AnyValue stored in "inputVar" will be added as an element to the field "outputVar".
 * The instruction fails if the destination field is not an array, or the type doesn't match the
 * array.
 */
class AddElement : public Instruction
{
public:
  AddElement();

  ~AddElement() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ADD_ELEMENT_H
