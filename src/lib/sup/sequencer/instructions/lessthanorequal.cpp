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

#include "lessthanorequal.h"

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_operations.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <iostream>
#include <string>

const std::string LEFT_VARIABLE_ATTR_NAME = "lhs";
const std::string RIGHT_VARIABLE_ATTR_NAME = "rhs";

namespace sup
{
namespace sequencer
{
const std::string LessThanOrEqual::Type = "LessThanOrEqual";

LessThanOrEqual::LessThanOrEqual()
  : Instruction(LessThanOrEqual::Type)
{
  AddAttributeDefinition(LEFT_VARIABLE_ATTR_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(RIGHT_VARIABLE_ATTR_NAME, sup::dto::StringType).SetMandatory();
}

LessThanOrEqual::~LessThanOrEqual() = default;

ExecutionStatus LessThanOrEqual::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue lhs;
  if (!GetValueFromAttributeName(*this, ws, ui, LEFT_VARIABLE_ATTR_NAME, lhs))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue rhs;
  if (!GetValueFromAttributeName(*this, ws, ui, RIGHT_VARIABLE_ATTR_NAME, rhs))
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
