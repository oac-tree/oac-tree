/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
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

#include "listen.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_utils.h>

#include <chrono>
#include <cstring>
#include <thread>

const std::string VARNAMES_ATTRIBUTE_NAME = "varNames";
const std::string FORCESUCCESS_ATTRIBUTE_NAME = "forceSuccess";

namespace sup
{
namespace sequencer
{
const std::string Listen::Type = "Listen";

Listen::Listen()
  : DecoratorInstruction(Listen::Type)
  , m_force_success{false}
  , m_var_changed{false}
  , m_var_names{}
  , m_var_cache{}
  , m_cb_guard{}
{
  AddAttributeDefinition(VARNAMES_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(FORCESUCCESS_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

Listen::~Listen() = default;

ExecutionStatus Listen::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (!m_cb_guard.IsValid())
  {
    m_var_changed = false;
    RegisterCallbacks(&ws, m_var_names);
  }
  auto child_status = GetChildStatus();
  if (IsFinishedStatus(child_status))
  {
    ResetChild();
    std::unique_lock<std::mutex> lk(m_mtx);
    m_cv.wait(lk, [this]{ return m_var_changed || IsHaltRequested(); });
    m_var_changed = false;
    lk.unlock();
    if (IsHaltRequested())
    {
      return ExecutionStatus::FAILURE;
    }
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
  m_cv.notify_one();
  ClearCallbacks();
}

void Listen::SetupImpl(const Procedure& proc)
{
  m_force_success = false;
  m_var_changed = true;
  if (HasAttribute(FORCESUCCESS_ATTRIBUTE_NAME))
  {
    m_force_success = GetAttributeValue<bool>(FORCESUCCESS_ATTRIBUTE_NAME);
  }
  m_var_names = instruction_utils::VariableNamesFromAttribute(*this, VARNAMES_ATTRIBUTE_NAME);
  m_var_cache.clear();
  for (const auto& var_name : m_var_names)
  {
    m_var_cache[var_name] = {};
  }
  return SetupChild(proc);
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

void Listen::UpdateCallback(const std::string& name, const sup::dto::AnyValue& val)
{
  std::lock_guard<std::mutex> lk(m_mtx);
  auto it = m_var_cache.find(name);
  if (it == m_var_cache.end() || it->second == val)
  {
    return;
  }
  m_var_changed = true;
  it->second = val;
  m_cv.notify_one();
}

void Listen::RegisterCallbacks(
    Workspace* ws, std::vector<std::string> var_names)
{
  m_cb_guard = ws->GetCallbackGuard(this);
  for (const auto& var_name : var_names)
  {
    ws->RegisterCallback(
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

}  // namespace sequencer

}  // namespace sup
