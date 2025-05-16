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

#include "user_confirmation.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/user_interface.h>

namespace sup
{
namespace oac_tree
{
const std::string UserConfirmation::Type = "UserConfirmation";

const std::string DEFAULT_OK_TEXT = "Ok";
const std::string DEFAULT_CANCEL_TEXT = "Cancel";

UserConfirmation::UserConfirmation()
  : Instruction(UserConfirmation::Type)
  , m_future{}
{
  AddAttributeDefinition(Constants::DESCRIPTION_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  AddAttributeDefinition(Constants::OK_TEXT_ATTRIBUTE_NAME).SetCategory(AttributeCategory::kBoth);
  AddAttributeDefinition(Constants::CANCEL_TEXT_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth);
}

UserConfirmation::~UserConfirmation() = default;

bool UserConfirmation::InitHook(UserInterface& ui, Workspace& ws)
{
  std::string description;
  if (!GetAttributeValueAs(Constants::DESCRIPTION_ATTRIBUTE_NAME, ws, ui, description))
  {
    return false;
  }
  std::string ok_text = DEFAULT_OK_TEXT;
  if (!GetAttributeValueAs(Constants::OK_TEXT_ATTRIBUTE_NAME, ws, ui, ok_text))
  {
    return false;
  }
  std::string cancel_text = DEFAULT_CANCEL_TEXT;
  if (!GetAttributeValueAs(Constants::CANCEL_TEXT_ATTRIBUTE_NAME, ws, ui, cancel_text))
  {
    return false;
  }
  auto metadata = CreateUserChoiceMetadata();
  metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, description);
  metadata.AddMember(Constants::USER_CHOICES_DIALOG_TYPE_NAME,
                     {sup::dto::UnsignedInteger32Type, dialog_type::kConfirmation});
  std::vector<std::string> options = { ok_text, cancel_text };
  m_future = CreateUserChoiceFuture(ui, *this, options, metadata);
  if (!m_future)
  {
    return false;
  }
  return true;
}

ExecutionStatus UserConfirmation::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (IsHaltRequested() || !m_future)
  {
    return ExecutionStatus::FAILURE;
  }
  return PollInputFuture(ui, ws);
}

void UserConfirmation::ResetHook(UserInterface& ui)
{
  Halt();
}

void UserConfirmation::HaltImpl()
{
  m_future.reset();
}

ExecutionStatus UserConfirmation::PollInputFuture(UserInterface& ui, Workspace& ws)
{
  if (!m_future->IsReady())
  {
    return ExecutionStatus::RUNNING;
  }
  auto reply = m_future->GetValue();
  auto [success, choice] = ParseUserChoiceReply(reply);
  if (!success)
  {
    std::string warning_message = InstructionWarningProlog(*this) +
      "did not receive valid choice";
    LogWarning(ui, warning_message);
    return ExecutionStatus::FAILURE;
  }
  return choice == 0 ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
