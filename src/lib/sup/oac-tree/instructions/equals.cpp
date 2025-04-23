/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
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

#include "equals.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

namespace sup
{
namespace oac_tree
{
const std::string Equals::Type = "Equals";

Equals::Equals()
  : Instruction(Equals::Type)
{
  AddAttributeDefinition(Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

Equals::~Equals() = default;

ExecutionStatus Equals::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue lhs;
  if (!GetAttributeValue(Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, lhs))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue rhs;
  if (!GetAttributeValue(Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, rhs))
  {
    return ExecutionStatus::FAILURE;
  }
  return lhs == rhs ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
