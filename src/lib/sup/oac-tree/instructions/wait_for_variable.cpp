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
  , m_blocking{false}
  , m_finish{}
  , m_updated{false}
  , m_mtx{}
  , m_cv{}
  , m_cb_guard{}
{
  AddAttributeDefinition(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  AddAttributeDefinition(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName);
  AddAttributeDefinition(Constants::BLOCKING_ATTRIBUTE_NAME, sup::dto::BooleanType)
    .SetCategory(AttributeCategory::kBoth);
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
  if (!GetAttributeValueAs(Constants::BLOCKING_ATTRIBUTE_NAME, ws, ui, m_blocking))
  {
    return false;
  }
  m_updated = true;  // Allow for initial checking of condition
  RegisterCallbacks(ws);
  return true;
}

ExecutionStatus WaitForVariable::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (m_blocking)
  {
    return ExecuteBlocking(ui, ws);
  }
  if (IsHaltRequested())
  {
    return HandleFinishedStatus(ExecutionStatus::FAILURE);
  }
  auto success = CheckUpdate() && CheckCondition(ui, ws);
  if (success)
  {
    return HandleFinishedStatus(ExecutionStatus::SUCCESS);
  }
  auto now = utils::GetNanosecsSinceEpoch();
  if (now > m_finish)
  {
    return HandleFinishedStatus(ExecutionStatus::FAILURE);
  }
  return ExecutionStatus::RUNNING;
}

void WaitForVariable::HaltImpl()
{
  {
    std::lock_guard<std::mutex> lock(m_mtx);  // Avoid lost wakeup
    m_cv.notify_one();
  }
  ClearCallbacks();
}

void WaitForVariable::ResetHook(UserInterface& ui)
{
  ClearCallbacks();
  m_blocking = false;
  m_finish = 0;
  m_updated = false;
}

ExecutionStatus WaitForVariable::ExecuteBlocking(UserInterface& ui, Workspace& ws)
{
  while (true)
  {
    auto now = utils::GetNanosecsSinceEpoch();
    if (now > m_finish)
    {
      return HandleFinishedStatus(ExecutionStatus::FAILURE);
    }
    auto duration_ns = m_finish - now;
    auto pred = [this]() {
      return m_updated || IsHaltRequested();
    };
    std::unique_lock<std::mutex> lk{m_mtx};
    auto result = m_cv.wait_for(lk, std::chrono::nanoseconds(duration_ns), pred);
    m_updated = false;
    lk.unlock();
    if (!result || IsHaltRequested())
    {
      break;
    }
    if (CheckCondition(ui, ws))
    {
      return HandleFinishedStatus(ExecutionStatus::SUCCESS);
    }
  }
  return ExecutionStatus::FAILURE;
}

void WaitForVariable::RegisterCallbacks(Workspace& ws)
{
  m_cb_guard = ws.GetCallbackGuard(this);
  auto callback = [this](const auto&, bool connected)
                  {
                    if (connected)
                    {
                      {
                        std::lock_guard<std::mutex> lk(m_mtx);
                        m_updated = true;
                      }
                      m_cv.notify_one();
                    }
                  };
  ws.RegisterCallback(GetAttributeString(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME),
                      callback, this);
  if (HasAttribute(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME))
  {
    ws.RegisterCallback(GetAttributeString(Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME),
                        callback, this);
  }
}

void WaitForVariable::ClearCallbacks()
{
  m_cb_guard = ScopeGuard{};
}

bool WaitForVariable::CheckUpdate()
{
  std::lock_guard<std::mutex> lk(m_mtx);
  if (m_updated)
  {
    m_updated = false;
    return true;
  }
  return false;
}

ExecutionStatus WaitForVariable::HandleFinishedStatus(ExecutionStatus status)
{
  ClearCallbacks();
  return status;
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
