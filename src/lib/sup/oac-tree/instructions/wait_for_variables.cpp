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

#include "wait_for_variables.h"

#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/workspace.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_utils.h>
#include <sup/oac-tree/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>

#include <numeric>

namespace
{
using sup::oac_tree::Workspace;
std::vector<std::string> GetVarNamesOfType(Workspace& ws, const std::string& var_type);
std::string ConcatenateVarNames(const std::vector<std::string>& var_names);
}  // unnamed namespace

namespace sup
{
namespace oac_tree
{
const std::string WaitForVariables::Type = "WaitForVariables";

WaitForVariables::WaitForVariables()
  : Instruction(WaitForVariables::Type)
  , m_blocking{false}
  , m_finish{}
  , m_var_names{}
  , m_unavailable_vars{}
  , m_updated{false}
  , m_mtx{}
  , m_cv{}
  , m_cb_guard{}
{
  (void)AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  (void)AddAttributeDefinition(Constants::VARIABLE_TYPE_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  (void)AddAttributeDefinition(Constants::BLOCKING_ATTRIBUTE_NAME, sup::dto::BooleanType)
    .SetCategory(AttributeCategory::kBoth);
}

WaitForVariables::~WaitForVariables() = default;

bool WaitForVariables::InitHook(UserInterface& ui, Workspace& ws)
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

ExecutionStatus WaitForVariables::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (m_blocking)
  {
    return ExecuteBlocking(ui, ws);
  }
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  auto success = CheckUpdate() && CheckCondition(ws);
  if (success)
  {
    return HandleFinishedStatus(ExecutionStatus::SUCCESS);
  }
  auto now = utils::GetNanosecsSinceEpoch();
  if (now > m_finish)
  {
    LogUnavailableVars(ui);
    return HandleFinishedStatus(ExecutionStatus::FAILURE);
  }
  return ExecutionStatus::RUNNING;
}

void WaitForVariables::HaltImpl()
{
  {
    std::lock_guard<std::mutex> lock(m_mtx);  // Avoid lost wakeup
    m_cv.notify_one();
  }
  ClearCallbacks();
}

void WaitForVariables::ResetHook(UserInterface&)
{
  ClearCallbacks();
  m_blocking = false;
  m_finish = 0;
  m_var_names.clear();
  m_unavailable_vars.clear();
  m_updated = false;
}

ExecutionStatus WaitForVariables::ExecuteBlocking(UserInterface&, Workspace& ws)
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
    if (CheckCondition(ws))
    {
      return HandleFinishedStatus(ExecutionStatus::SUCCESS);
    }
  }
  return ExecutionStatus::FAILURE;
}

void WaitForVariables::RegisterCallbacks(Workspace& ws)
{
  const auto var_type = GetAttributeString(Constants::VARIABLE_TYPE_ATTRIBUTE_NAME);
  m_var_names = GetVarNamesOfType(ws, var_type);
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
  for (const auto& var_name : m_var_names)
  {
    (void)ws.RegisterCallback(var_name, callback, this);
  }
}

void WaitForVariables::ClearCallbacks()
{
  m_cb_guard = ScopeGuard{};
}

bool WaitForVariables::CheckUpdate()
{
  std::lock_guard<std::mutex> lk(m_mtx);
  if (m_updated)
  {
    m_updated = false;
    return true;
  }
  return false;
}

ExecutionStatus WaitForVariables::HandleFinishedStatus(ExecutionStatus status)
{
  ClearCallbacks();
  return status;
}

bool WaitForVariables::CheckCondition(Workspace& ws) const
{
  m_unavailable_vars.clear();
  for (const auto& var_name : m_var_names)
  {
    const auto* var = ws.GetVariable(var_name);
    if (var == nullptr || !var->IsAvailable())
    {
      m_unavailable_vars.push_back(var_name);
    }
  }
  return m_unavailable_vars.empty();
}

void WaitForVariables::LogUnavailableVars(UserInterface& ui) const
{
  const std::string warning_message = InstructionWarningProlog(*this)
    + " encountered unavailable variables: " + ConcatenateVarNames(m_unavailable_vars);
  LogWarning(ui, warning_message);
}

}  // namespace oac_tree

}  // namespace sup

namespace
{
std::vector<std::string> GetVarNamesOfType(Workspace& ws, const std::string& var_type)
{
  std::vector<std::string> result;
  for (const auto& var_name : ws.VariableNames())
  {
    const auto var = ws.GetVariable(var_name);
    if (var != nullptr && var->GetType() == var_type)
    {
      result.push_back(var_name);
    }
  }
  return result;
}
std::string ConcatenateVarNames(const std::vector<std::string>& var_names)
{
  auto op = [](const std::string& left, const std::string& right){
    return left + ", " + right;
  };
  std::string result = std::accumulate(var_names.begin(), var_names.end(), std::string{}, op);
  return result.size() > 1 ? result.substr(2) : "";
}

}  // unnamed namespace

