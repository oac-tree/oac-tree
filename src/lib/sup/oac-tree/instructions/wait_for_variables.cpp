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
  , m_finish{}
  , m_var_names{}
{
  AddAttributeDefinition(Constants::TIMEOUT_SEC_ATTRIBUTE_NAME, sup::dto::Float64Type)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  AddAttributeDefinition(Constants::VARIABLE_TYPE_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
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
  const auto var_type = GetAttributeString(Constants::VARIABLE_TYPE_ATTRIBUTE_NAME);
  m_var_names = GetVarNamesOfType(ws, var_type);
  return true;
}

ExecutionStatus WaitForVariables::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  auto var_names = UnavailableVars(ws, m_var_names);
  if (var_names.empty())
  {
    return ExecutionStatus::SUCCESS;
  }
  auto now = utils::GetNanosecsSinceEpoch();
  if (m_finish > now)
  {
    return ExecutionStatus::RUNNING;
  }
  const std::string warning_message = InstructionWarningProlog(*this)
    + " encountered unavailable variables: " + ConcatenateVarNames(var_names);
  LogWarning(ui, warning_message);
  return ExecutionStatus::FAILURE;
}

void WaitForVariables::ResetHook(UserInterface& ui)
{
  m_finish = 0;
  m_var_names.clear();
}

std::vector<std::string> WaitForVariables::UnavailableVars(
  Workspace& ws, const std::vector<std::string>& var_names) const
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
    return left + ", " + right;
  };
  std::string result = std::accumulate(var_names.begin(), var_names.end(), std::string{}, op);
  return result.size() > 1 ? result.substr(2) : "";
}

}  // unnamed namespace

