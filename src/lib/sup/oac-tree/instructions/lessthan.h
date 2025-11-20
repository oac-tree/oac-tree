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

#ifndef SUP_OAC_TREE_LESSTHAN_H_
#define SUP_OAC_TREE_LESSTHAN_H_

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction checking if a variable is smaller than an other one.
 *
 * @details Check if variable identified with 'lhs' attribute is smaller than one with 'rhs'
 * attribute.
 */
class LessThan : public Instruction
{
public:
  LessThan();

  ~LessThan() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
};

template <>
inline DocumentationEntry InstructionDocFor<LessThan>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction checking if a variable is smaller than an other one" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_LESSTHAN_H_
