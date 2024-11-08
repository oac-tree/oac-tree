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

#include "input.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>

#include <chrono>
#include <thread>

namespace sup
{
namespace sequencer
{
const std::string Input::Type = "Input";

Input::Input()
  : Instruction(Input::Type)
{
  AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::DESCRIPTION_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth);
}

Input::~Input() = default;

ExecutionStatus Input::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  std::string description;
  if (!GetAttributeValueAs(Constants::DESCRIPTION_ATTRIBUTE_NAME, ws, ui, description))
  {
    return ExecutionStatus::FAILURE;
  }
  auto input_request = CreateUserValueRequest(value, description);
  auto future = ui.RequestUserInput(input_request);
  if (!future->IsValid())
  {
    std::string error_message = InstructionErrorProlog(*this) +
      "could not retrieve a valid future for user input";
    LogError(ui, error_message);
    return ExecutionStatus::FAILURE;
  }
  while (!IsHaltRequested() && !future->IsReady())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(DefaultSettings::TIMING_ACCURACY_MS));
  }
  if (IsHaltRequested())
  {
    return ExecutionStatus::FAILURE;
  }
  auto reply = future->GetValue();
  auto parsed = ParseUserValueReply(reply);
  if (!parsed.first)
  {
    std::string warning_message = InstructionWarningProlog(*this) +
      "did not receive compatible user value for field [" +
      GetAttributeString(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME) + "[ in workspace";
    LogWarning(ui, warning_message);
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME,
                                 parsed.second))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
