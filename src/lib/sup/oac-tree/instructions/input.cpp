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

#include "input.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue_helper.h>

namespace sup
{
namespace oac_tree
{
const std::string Input::Type = "Input";

Input::Input()
  : Instruction(Input::Type)
  , m_future{}
{
  AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::DESCRIPTION_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth);
}

Input::~Input() = default;

bool Input::InitHook(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return false;
  }
  std::string description;
  if (!GetAttributeValueAs(Constants::DESCRIPTION_ATTRIBUTE_NAME, ws, ui, description))
  {
    return false;
  }
  m_future = CreateUserValueFuture(ui, *this, value, description);
  if (!m_future)
  {
    return false;
  }
  return true;
}

ExecutionStatus Input::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (IsHaltRequested() || !m_future)
  {
    // If the instruction is halted, we need to cancel a possible request.
    m_future.reset();
    return ExecutionStatus::FAILURE;
  }
  return PollInputFuture(ui, ws);
}

void Input::ResetHook(UserInterface& ui)
{
  m_future.reset();
}

ExecutionStatus Input::PollInputFuture(UserInterface& ui, Workspace& ws)
{
  if (!m_future->IsReady())
  {
    return ExecutionStatus::RUNNING;
  }
  else
  {
    auto reply = m_future->GetValue();
    m_future.reset();  // Immediately cleanup the future to put the instruction in a valid state
                       // for re-execution
    auto [success, user_value] = ParseUserValueReply(reply);
    if (!success)
    {
      std::string warning_message =
          InstructionWarningProlog(*this) + "did not receive compatible user value for field ["
          + GetAttributeString(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME) + "] in workspace";
      LogWarning(ui, warning_message);
      return ExecutionStatus::FAILURE;
    }
    if (!SetValueFromAttributeName(*this, ws, ui, Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME,
                                   user_value))
    {
      return ExecutionStatus::FAILURE;
    }
    return ExecutionStatus::SUCCESS;
  }
}

}  // namespace oac_tree

}  // namespace sup
