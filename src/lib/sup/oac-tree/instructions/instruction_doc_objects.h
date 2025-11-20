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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_INSTRUCTION_DOC_OBJECTS_H_
#define SUP_OAC_TREE_INSTRUCTION_DOC_OBJECTS_H_

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction_doc.h>

namespace sup
{
namespace oac_tree
{

class Choice;

template <>
InstructionDoc InstructionDocFor<Choice>()
{
  InstructionDoc doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Executes a subset of children depending on a variable value" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_DOC_OBJECTS_H_
