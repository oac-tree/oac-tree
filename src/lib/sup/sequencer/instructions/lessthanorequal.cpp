/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Ricardo Torres (Ext)
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

#include "lessthanorequal.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/dto/basic_scalar_types.h>

#include <iostream>
#include <string>

namespace sup
{
namespace sequencer
{
const std::string LessThanOrEqual::Type = "LessThanOrEqual";

LessThanOrEqual::LessThanOrEqual()
  : Instruction(LessThanOrEqual::Type)
{
  AddAttributeDefinition(Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

LessThanOrEqual::~LessThanOrEqual() = default;

ExecutionStatus LessThanOrEqual::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
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

  return result == sup::dto::CompareResult::Less || result == sup::dto::CompareResult::Equivalent
             ? ExecutionStatus::SUCCESS
             : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
