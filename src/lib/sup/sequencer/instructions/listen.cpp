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

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>

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
  , force_success{false}
  , var_changed{false}
  , var_names{}
  , var_cache{}
  , cb_guard{}
{
  AddAttributeDefinition(VARNAMES_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(FORCESUCCESS_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

Listen::~Listen() = default;

ExecutionStatus Listen::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (!cb_guard.IsValid())
  {
    var_changed = false;
    RegisterCallbacks(&ws, var_names);
  }
  auto child_status = GetChildStatus();
  if (IsFinishedStatus(child_status))
  {
    ResetChild();
    std::unique_lock<std::mutex> lk(mx);
    cv.wait(lk, [this]{ return var_changed || IsHaltRequested(); });
    var_changed = false;
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
  cv.notify_one();
  ClearCallbacks();
}

void Listen::SetupImpl(const Procedure& proc)
{
  force_success = false;
  var_changed = true;
  if (HasAttribute(FORCESUCCESS_ATTRIBUTE_NAME))
  {
    force_success = GetAttributeValue<bool>(FORCESUCCESS_ATTRIBUTE_NAME);
  }
  var_names = VariableNames();
  var_cache.clear();
  for (const auto& var_name : var_names)
  {
    var_cache[var_name] = {};
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
  if (force_success && child_status == ExecutionStatus::FAILURE)
  {
    return ExecutionStatus::NOT_FINISHED;
  }
  return child_status;
}

std::vector<std::string> Listen::VariableNames() const
{
  auto var_names_attr = GetAttributeValue<std::string>(VARNAMES_ATTRIBUTE_NAME);
  std::vector<std::string> result;
  size_t pos = var_names_attr.find_first_not_of(DefaultSettings::VARNAME_DELIMITER);
  while (pos != std::string::npos)
  {
    auto next = var_names_attr.find_first_of(DefaultSettings::VARNAME_DELIMITER, pos);
    auto var_name = var_names_attr.substr(pos, next - pos);
    if (!var_name.empty())
    {
      result.push_back(var_name);
    }
    pos = var_names_attr.find_first_not_of(DefaultSettings::VARNAME_DELIMITER, next);
  }
  return result;
}

void Listen::UpdateCallback(const std::string& name, const sup::dto::AnyValue& val)
{
  std::lock_guard<std::mutex> lk(mx);
  auto it = var_cache.find(name);
  if (it == var_cache.end() || it->second == val)
  {
    return;
  }
  var_changed = true;
  it->second = val;
  cv.notify_one();
}

void Listen::RegisterCallbacks(
    Workspace* ws, std::vector<std::string> var_names)
{
  cb_guard = ws->GetCallbackGuard(this);
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
  cb_guard = ScopeGuard{};
}

}  // namespace sequencer

}  // namespace sup
