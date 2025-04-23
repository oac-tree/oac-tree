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

#include "condition.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace oac_tree
{
const std::string Condition::Type = "Condition";

Condition::Condition()
  : Instruction(Condition::Type)
{
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

Condition::~Condition() = default;

ExecutionStatus Condition::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::boolean result = false;
  if (!GetAttributeValueAs(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, result))
  {
    return ExecutionStatus::FAILURE;
  }
  return result ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
