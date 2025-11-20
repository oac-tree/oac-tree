/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#ifndef SUP_OAC_TREE_GREATERTHAN_H_
#define SUP_OAC_TREE_GREATERTHAN_H_

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction checking the if a variable is greater than the other
 *
 * @details Check if variable identified with 'lhs' attribute is greater than one with 'rhs'
 * attribute.
 */
class GreaterThan : public Instruction
{
public:
  GreaterThan();

  ~GreaterThan() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
};

template <>
inline DocumentationEntry InstructionDocFor<GreaterThan>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking the if a variable is greater than the other" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_GREATERTHAN_H_
