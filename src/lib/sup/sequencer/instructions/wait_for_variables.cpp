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

#include "wait_for_variables.h"

#include "sup/sequencer/execution_status.h"
#include "sup/sequencer/workspace.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_utils.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>

namespace
{
using sup::sequencer::Workspace;
std::vector<std::string> GetVarNamesOfType(Workspace& ws, const std::string& var_type);
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
  bool success = false;
  auto predicate = [&]{
                        success = CheckCondition(ui, ws, var_names);
                        return IsHaltRequested() || success;
                      };
  std::mutex mx;
  std::unique_lock<std::mutex> lk(mx);
  auto result = cv.wait_for(lk, std::chrono::nanoseconds(timeout_ns), predicate);

  return success ? ExecutionStatus::SUCCESS
                 : ExecutionStatus::FAILURE;
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

bool WaitForVariables::CheckCondition(UserInterface& ui, Workspace& ws,
                                      const std::vector<std::string>& var_names) const
{
  for (const auto& var_name : var_names)
  {
    sup::dto::AnyValue var_value;
    if (!GetAttributeValue(Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, var_value))
    {
      return false;
    }
    if (sup::dto::IsEmptyValue(var_value))
    {
      return false;
    }
  }
  return true;
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
}  // unnamed namespace

