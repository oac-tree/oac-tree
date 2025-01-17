/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Ricardo Torres (EXT)
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

#include "greaterthanorequal.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/dto/basic_scalar_types.h>

#include <iostream>
#include <string>

namespace sup
{
namespace oac_tree
{
const std::string GreaterThanOrEqual::Type = "GreaterThanOrEqual";

GreaterThanOrEqual::GreaterThanOrEqual()
  : Instruction(GreaterThanOrEqual::Type)
{
  AddAttributeDefinition(Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

GreaterThanOrEqual::~GreaterThanOrEqual() = default;

ExecutionStatus GreaterThanOrEqual::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
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

  sup::dto::CompareResult result = sup::dto::Compare(lhs, rhs);

  return result == sup::dto::CompareResult::Greater || result == sup::dto::CompareResult::Equivalent
             ? ExecutionStatus::SUCCESS
             : ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
