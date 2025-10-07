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

#include "listen.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_utils.h>

namespace sup
{
namespace oac_tree
{
const std::string Listen::Type = "Listen";

Listen::Listen()
  : DecoratorInstruction(Listen::Type)
  , m_force_success{false}
  , m_blocking{false}
  , m_var_cache{}
  , m_var_changed{false}
  , m_mtx{}
  , m_cv{}
  , m_cb_guard{}
{
  (void)AddAttributeDefinition(Constants::VARIABLE_NAMES_ATTRIBUTE_NAME).SetMandatory();
  (void)AddAttributeDefinition(Constants::FORCE_SUCCESS_ATTRIBUTE_NAME, sup::dto::BooleanType)
    .SetCategory(AttributeCategory::kBoth);
  (void)AddAttributeDefinition(Constants::BLOCKING_ATTRIBUTE_NAME, sup::dto::BooleanType)
    .SetCategory(AttributeCategory::kBoth);
}

Listen::~Listen() = default;

bool Listen::InitHook(UserInterface& ui, Workspace& ws)
{
  auto var_names =
    instruction_utils::VariableNamesFromAttribute(*this, Constants::VARIABLE_NAMES_ATTRIBUTE_NAME);
  InitVariableCache(var_names);
  if (!GetAttributeValueAs(Constants::FORCE_SUCCESS_ATTRIBUTE_NAME, ws, ui, m_force_success))
  {
    return false;
  }
  if (!GetAttributeValueAs(Constants::BLOCKING_ATTRIBUTE_NAME, ws, ui, m_blocking))
  {
    return false;
  }
  RegisterCallbacks(std::addressof(ws), var_names);
  // m_var_changed is not set to true, since the child will always be executed when it is not
  // in a finished status
  return true;
}

ExecutionStatus Listen::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto child_status = GetChildStatus();
  if (IsFinishedStatus(child_status))
  {
    std::unique_lock<std::mutex> lk(m_mtx);
    if (m_blocking)
    {
      m_cv.wait(lk, [this]() { return m_var_changed || IsHaltRequested(); });
    }
    else if (!m_var_changed && !IsHaltRequested())
    {
      return ExecutionStatus::RUNNING;
    }
    else
    {
      // Avoid static analysis warning
    }
    m_var_changed = false;
    lk.unlock();
    ResetChild(ui);
  }
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  ExecuteChild(ui, ws);
  auto status = CalculateStatus();
  if (IsFinishedStatus(status))
  {
    ClearCallbacks();
  }
  return status;
}

void Listen::HaltImpl()
{
  HaltChild();
  {
    std::lock_guard<std::mutex> lock(m_mtx);  // Avoid lost wakeup
    m_cv.notify_one();
  }
  ClearCallbacks();
}

void Listen::ResetHook(UserInterface& ui)
{
  ClearCallbacks();
  m_force_success = false;
  m_blocking = false;
  m_var_cache.clear();
  m_var_changed = false;
  ResetChild(ui);
}

void Listen::InitVariableCache(const std::vector<std::string>& var_names)
{
  m_var_cache.clear();
  for (const auto& var_name : var_names)
  {
    m_var_cache[var_name] = {};
  }
}

void Listen::UpdateCallback(const std::string& name, const sup::dto::AnyValue& val)
{
  {
    std::lock_guard<std::mutex> lk(m_mtx);
    auto it = m_var_cache.find(name);
    if (it == m_var_cache.end() || it->second == val)
    {
      return;
    }
    m_var_changed = true;
    it->second = val;
  }
  m_cv.notify_one();
}

void Listen::RegisterCallbacks(
    Workspace* ws, std::vector<std::string> var_names)
{
  m_cb_guard = ws->GetCallbackGuard(this);
  for (const auto& var_name : var_names)
  {
    (void)ws->RegisterCallback(
        var_name,
        [this, var_name](const sup::dto::AnyValue& val, bool)
        {
          UpdateCallback(var_name, val);
        }, this);
  }
}

void Listen::ClearCallbacks()
{
  m_cb_guard = ScopeGuard{};
}

ExecutionStatus Listen::CalculateStatus() const
{
  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    return ExecutionStatus::NOT_FINISHED;
  }
  if (m_force_success && child_status == ExecutionStatus::FAILURE)
  {
    return ExecutionStatus::NOT_FINISHED;
  }
  return child_status;
}

}  // namespace oac_tree

}  // namespace sup
