/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "wait_for_variables.h"

#include "sup/sequencer/execution_status.h"
#include "sup/sequencer/workspace.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_utils.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>

#include <numeric>

namespace
{
using sup::sequencer::Workspace;
std::vector<std::string> GetVarNamesOfType(Workspace& ws, const std::string& var_type);
std::string ConcatenateVarNames(const std::vector<std::string>& var_names);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{
const std::string WaitForVariables::Type = "WaitForVariables";

WaitForVariables::WaitForVariables()
  : Instruction(WaitForVariables::Type)
{
  AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  AddAttributeDefinition(Constants::VARIABLE_TYPE_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
}

WaitForVariables::~WaitForVariables() = default;

ExecutionStatus WaitForVariables::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::int64 timeout_ns;
  if (!instruction_utils::GetVariableTimeoutAttribute(
                *this, ui, ws, Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, timeout_ns))
  {
    return ExecutionStatus::FAILURE;
  }
  const auto var_type = GetAttributeString(Constants::VARIABLE_TYPE_ATTRIBUTE_NAME);
  const auto var_names = GetVarNamesOfType(ws, var_type);

  // Register callbacks:
  std::condition_variable cv;
  auto cb_guard = ws.GetCallbackGuard(this);
  RegisterCallbacks(ws, cv, this, var_names);

  // Wait for condition to be satisfied, halt or timeout:
  std::vector<std::string> unavailable_vars{};
  auto predicate = [&]{
                        unavailable_vars = UnavailableVars(ui, ws, var_names);
                        return IsHaltRequested() || unavailable_vars.empty();
                      };
  std::mutex mx;
  std::unique_lock<std::mutex> lk(mx);
  (void)cv.wait_for(lk, std::chrono::nanoseconds(timeout_ns), predicate);

  if (unavailable_vars.empty())
  {
    return ExecutionStatus::SUCCESS;
  }
  LogWarning(ui, ConcatenateVarNames(unavailable_vars));
  return ExecutionStatus::FAILURE;
}

void WaitForVariables::RegisterCallbacks(
  Workspace& ws, std::condition_variable& cv, void* listener,
  const std::vector<std::string>& var_names) const
{
  auto callback = [&](const sup::dto::AnyValue&, bool)
                  {
                    cv.notify_one();
                  };
  for (const auto& var_name : var_names)
  {
    ws.RegisterCallback(var_name, callback, listener);
  }
}

std::vector<std::string> WaitForVariables::UnavailableVars(
  UserInterface& ui, Workspace& ws, const std::vector<std::string>& var_names) const
{
  std::vector<std::string> unavailable_vars{};
  for (const auto& var_name : var_names)
  {
    const auto* var = ws.GetVariable(var_name);
    if (var == nullptr || !var->IsAvailable())
    {
      unavailable_vars.push_back(var_name);
    }
  }
  return unavailable_vars;
}

}  // namespace sequencer

}  // namespace sup

namespace
{
std::vector<std::string> GetVarNamesOfType(Workspace& ws, const std::string& var_type)
{
  std::vector<std::string> result;
  for (const auto var_name : ws.VariableNames())
  {
    const auto var = ws.GetVariable(var_name);
    if (var->GetType() == var_type)
    {
      result.push_back(var_name);
    }
  }
  return result;
}
std::string ConcatenateVarNames(const std::vector<std::string>& var_names)
{
  auto op = [](const std::string& left, const std::string& right){
    return left + "," + right;
  };
  std::string result = std::accumulate(var_names.begin(), var_names.end(), std::string{}, op);
  return result.size() > 0 ? result.substr(1) : "";
}

}  // unnamed namespace

