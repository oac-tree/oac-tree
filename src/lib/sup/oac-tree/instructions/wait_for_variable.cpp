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

#include "wait_for_variable.h"

#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/workspace.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction_utils.h>
#include <sup/oac-tree/generic_utils.h>

namespace sup
{
namespace oac_tree
{
const std::string WaitForVariable::Type = "WaitForVariable";

WaitForVariable::WaitForVariable()
  : Instruction(WaitForVariable::Type)
  , m_finish{}
{
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  AddAttributeDefinition(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName);
}

WaitForVariable::~WaitForVariable() = default;

bool WaitForVariable::InitHook(UserInterface& ui, Workspace& ws)
{
  sup::dto::int64 timeout_ns{};
  if (!instruction_utils::GetVariableTimeoutAttribute(
            *this, ui, ws, Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, timeout_ns))
  {
    return false;
  }
  m_finish = utils::GetNanosecsSinceEpoch() + timeout_ns;
  return true;
}

ExecutionStatus WaitForVariable::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  auto success = CheckCondition(ui, ws);
  if (success)
  {
    return ExecutionStatus::SUCCESS;
  }
  auto now = utils::GetNanosecsSinceEpoch();
  if (m_finish > now)
  {
    return ExecutionStatus::RUNNING;
  }
  return ExecutionStatus::FAILURE;
}

void WaitForVariable::ResetHook(UserInterface& ui)
{
  m_finish = 0;
}

bool WaitForVariable::SuccessCondition(
  bool var_available, const sup::dto::AnyValue& var_value,
  bool other_available, const sup::dto::AnyValue& other_value) const
{
  if (!var_available || sup::dto::IsEmptyValue(var_value))
  {
    return false;
  }
  if (!HasAttribute(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME))
  {
    return true;
  }
  if (!other_available)
  {
    return false;
  }
  return var_value == other_value;
}

bool WaitForVariable::CheckCondition(UserInterface& ui, Workspace& ws) const
{
  sup::dto::AnyValue var_value;
  sup::dto::AnyValue other_value;

  bool var_available =
    GetAttributeValue(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, var_value);
  bool other_available = false;
  if (HasAttribute(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME))
  {
    other_available =
      GetAttributeValue(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, other_value);
  }
  return SuccessCondition(var_available, var_value, other_available, other_value);
}

}  // namespace oac_tree

}  // namespace sup
