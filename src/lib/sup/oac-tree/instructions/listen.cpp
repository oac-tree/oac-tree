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
  , m_var_cache{}
{
  AddAttributeDefinition(Constants::VARIABLE_NAMES_ATTRIBUTE_NAME).SetMandatory();
  AddAttributeDefinition(Constants::FORCE_SUCCESS_ATTRIBUTE_NAME, sup::dto::BooleanType)
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
  return true;
}

ExecutionStatus Listen::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  auto child_status = GetChildStatus();
  if (IsExecutingStatus(child_status))
  {
    ExecuteChild(ui, ws);
    return CalculateStatus();
  }
  if (IsFinishedStatus(child_status))
  {
    ResetChild(ui);
  }
  auto cache_changed = UpdateVariableCache(ws);
  if (!cache_changed)
  {
    return ExecutionStatus::RUNNING;
  }
  ExecuteChild(ui, ws);
  return CalculateStatus();
}

void Listen::ResetHook(UserInterface& ui)
{
  m_force_success = false;
  m_var_cache.clear();
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

bool Listen::UpdateVariableCache(Workspace& ws)
{
  auto cache_changed = false;
  for (auto& [var_name, var_value] : m_var_cache)
  {
    sup::dto::AnyValue new_value;
    if (!ws.GetValue(var_name, new_value))
    {
      continue;
    }
    if (var_value == new_value)
    {
      continue;
    }
    var_value = new_value;
    cache_changed = true;
  }
  return cache_changed;
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
