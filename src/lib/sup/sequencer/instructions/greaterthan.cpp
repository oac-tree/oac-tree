/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "greaterthan.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>
#include <sup/dto/anyvalue_operations.h>

const std::string LEFT_VARIABLE_ATTR_NAME = "leftVar";
const std::string RIGHT_VARIABLE_ATTR_NAME = "rightVar";

namespace sup
{
namespace sequencer
{
const std::string GreaterThan::Type = "GreaterThan";

GreaterThan::GreaterThan()
  : Instruction(GreaterThan::Type)
{
  AddAttributeDefinition(LEFT_VARIABLE_ATTR_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(RIGHT_VARIABLE_ATTR_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

GreaterThan::~GreaterThan() = default;

ExecutionStatus GreaterThan::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue lhs;
  if (!GetAttributeValue(LEFT_VARIABLE_ATTR_NAME, ws, ui, lhs))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue rhs;
  if (!GetAttributeValue(RIGHT_VARIABLE_ATTR_NAME, ws, ui, rhs))
  {
    return ExecutionStatus::FAILURE;
  }

  return sup::dto::Compare(lhs, rhs) == sup::dto::CompareResult::Greater ? ExecutionStatus::SUCCESS
                                                                         : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
