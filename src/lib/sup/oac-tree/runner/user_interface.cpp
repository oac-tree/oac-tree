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

#include <sup/oac-tree/user_interface.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/log_severity.h>

namespace sup
{
namespace oac_tree
{
const sup::dto::AnyType USER_CHOICE_METADATA_ANYTYPE =
  sup::dto::EmptyStructType(Constants::USER_CHOICES_METADATA_NAME);

UserInterface::~UserInterface() = default;

void LogError(UserInterface& ui, const std::string& message)
{
  ui.Log(log::SUP_SEQ_LOG_ERR, message);
}

void LogWarning(UserInterface& ui, const std::string& message)
{
  ui.Log(log::SUP_SEQ_LOG_WARNING, message);
}

DefaultUserInterface::DefaultUserInterface() = default;

DefaultUserInterface::~DefaultUserInterface() = default;

void DefaultUserInterface::UpdateInstructionStatus(const Instruction*)
{}

void DefaultUserInterface::VariableUpdated(const std::string&, const sup::dto::AnyValue&, bool)
{}

bool DefaultUserInterface::PutValue(const sup::dto::AnyValue&, const std::string&)
{
  return false;
}

std::unique_ptr<IUserInputFuture> DefaultUserInterface::RequestUserInput(
  const UserInputRequest& request)
{
  (void)request;
  return std::unique_ptr<IUserInputFuture>{new UnsupportedInputFuture{}};
}

void DefaultUserInterface::Message(const std::string&)
{}

void DefaultUserInterface::Log(int, const std::string&)
{}

sup::dto::AnyValue CreateUserChoiceMetadata()
{
  sup::dto::AnyValue result{USER_CHOICE_METADATA_ANYTYPE};
  return result;
}

bool IsUserChoiceMetadata(const sup::dto::AnyValue& metadata)
{
  if (!sup::dto::IsStructValue(metadata))
  {
    return false;
  }
  if (metadata.HasField(Constants::USER_CHOICES_TEXT_NAME) &&
      (metadata[Constants::USER_CHOICES_TEXT_NAME].GetType()
        != sup::dto::StringType))
  {
    return false;
  }
  if (metadata.HasField(Constants::USER_CHOICES_DIALOG_TYPE_NAME) &&
      (metadata[Constants::USER_CHOICES_DIALOG_TYPE_NAME].GetType()
        != sup::dto::UnsignedInteger32Type))
  {
    return false;
  }
  if (metadata.HasField(Constants::USER_CHOICES_MODAL_NAME) &&
      (metadata[Constants::USER_CHOICES_MODAL_NAME].GetType()
        != sup::dto::BooleanType))
  {
    return false;
  }
  if (metadata.HasField(Constants::USER_CHOICES_TITLE_NAME) &&
      (metadata[Constants::USER_CHOICES_TITLE_NAME].GetType()
        != sup::dto::StringType))
  {
    return false;
  }
  if (metadata.HasField(Constants::USER_CHOICES_INFORMATIVE_TEXT_NAME) &&
      (metadata[Constants::USER_CHOICES_INFORMATIVE_TEXT_NAME].GetType()
        != sup::dto::StringType))
  {
    return false;
  }
  if (metadata.HasField(Constants::USER_CHOICES_DETAILS_NAME) &&
      (metadata[Constants::USER_CHOICES_DETAILS_NAME].GetType()
        != sup::dto::StringType))
  {
    return false;
  }
  return true;
}

std::string GetMainTextFromMetadata(const sup::dto::AnyValue& metadata)
{
  if (metadata.HasField(Constants::USER_CHOICES_TEXT_NAME) &&
      (metadata[Constants::USER_CHOICES_TEXT_NAME].GetType() == sup::dto::StringType))
  {
    return metadata[Constants::USER_CHOICES_TEXT_NAME].As<std::string>();
  }
  return {};
}

std::pair<bool, sup::dto::AnyValue> GetBlockingUserValue(UserInterface& ui,
                                                         const sup::dto::AnyValue& value,
                                                         const std::string& description)
{
  std::pair<bool, sup::dto::AnyValue> failure{ false, {} };
  auto input_request = CreateUserValueRequest(value, description);
  auto future = ui.RequestUserInput(input_request);
  if (!future->IsValid())
  {
    std::string error_message = "Could not retrieve a valid future for user value input";
    LogError(ui, error_message);
    return failure;
  }
  double timeout_s = DefaultSettings::TIMING_ACCURACY_MS / 1000.0;
  while (!future->WaitFor(timeout_s)) {}
  auto reply = future->GetValue();
  return ParseUserValueReply(reply);
}

std::pair<bool, int> GetBlockingUserChoice(UserInterface& ui,
                                           const std::vector<std::string>& options,
                                           const sup::dto::AnyValue& metadata)
{
  std::pair<bool, int> failure{ false, -1 };
  auto input_request = CreateUserChoiceRequest(options, metadata);
  auto future = ui.RequestUserInput(input_request);
  if (!future->IsValid())
  {
    std::string error_message = "Could not retrieve a valid future for user choice input";
    LogError(ui, error_message);
    return failure;
  }
  double timeout_s = DefaultSettings::TIMING_ACCURACY_MS / 1000.0;
  while (!future->WaitFor(timeout_s)) {}
  auto reply = future->GetValue();
  return ParseUserChoiceReply(reply);
}

std::pair<bool, sup::dto::AnyValue> GetInterruptableUserValue(
  UserInterface& ui, const Instruction& instr, const sup::dto::AnyValue& value,
  const std::string& description)
{
  std::pair<bool, sup::dto::AnyValue> failure{ false, {} };
  auto input_request = CreateUserValueRequest(value, description);
  auto future = ui.RequestUserInput(input_request);
  if (!future->IsValid())
  {
    std::string error_message = InstructionErrorProlog(instr) +
      "could not retrieve a valid future for user input";
    LogError(ui, error_message);
    return failure;
  }
  double timeout_s = DefaultSettings::TIMING_ACCURACY_MS / 1000.0;
  while (!instr.IsHaltRequested() && !future->WaitFor(timeout_s)) {}
  if (instr.IsHaltRequested())
  {
    return failure;
  }
  auto reply = future->GetValue();
  return ParseUserValueReply(reply);
}

std::pair<bool, int> GetInterruptableUserChoice(UserInterface& ui, const Instruction& instr,
                                                const std::vector<std::string>& options,
                                                const sup::dto::AnyValue& metadata)
{
  std::pair<bool, int> failure{ false, -1 };
  auto input_request = CreateUserChoiceRequest(options, metadata);
  auto future = ui.RequestUserInput(input_request);
  if (!future->IsValid())
  {
    std::string error_message = InstructionErrorProlog(instr) +
      "could not retrieve a valid future for user input";
    LogError(ui, error_message);
    return failure;
  }
  double timeout_s = DefaultSettings::TIMING_ACCURACY_MS / 1000.0;
  while (!instr.IsHaltRequested() && !future->WaitFor(timeout_s)) {}
  if (instr.IsHaltRequested())
  {
    return failure;
  }
  auto reply = future->GetValue();
  return ParseUserChoiceReply(reply);
}

}  // namespace oac_tree

}  // namespace sup
