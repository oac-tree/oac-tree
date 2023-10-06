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

#include "user_confirmation.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
{
const std::string UserConfirmation::Type = "UserConfirmation";

const std::string DESCRIPTION_ATTRIBUTE = "description";
const std::string OK_ATTRIBUTE = "okText";
const std::string CANCEL_ATTRIBUTE = "cancelText";

const std::string DEFAULT_OK_TEXT = "Ok";
const std::string DEFAULT_CANCEL_TEXT = "Cancel";

UserConfirmation::UserConfirmation()
  : Instruction(UserConfirmation::Type)
{
  AddAttributeDefinition(DESCRIPTION_ATTRIBUTE)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  AddAttributeDefinition(OK_ATTRIBUTE).SetCategory(AttributeCategory::kBoth);
  AddAttributeDefinition(CANCEL_ATTRIBUTE).SetCategory(AttributeCategory::kBoth);
}

UserConfirmation::~UserConfirmation() = default;

ExecutionStatus UserConfirmation::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  std::string description;
  if (!GetAttributeValueAs(DESCRIPTION_ATTRIBUTE, ws, ui, description))
  {
    return ExecutionStatus::FAILURE;
  }
  std::string ok_text = DEFAULT_OK_TEXT;
  if (!GetAttributeValueAs(OK_ATTRIBUTE, ws, ui, ok_text))
  {
    return ExecutionStatus::FAILURE;
  }
  std::string cancel_text = DEFAULT_CANCEL_TEXT;
  if (!GetAttributeValueAs(CANCEL_ATTRIBUTE, ws, ui, cancel_text))
  {
    return ExecutionStatus::FAILURE;
  }
  auto metadata = CreateUserChoiceMetadata();
  metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, description);
  metadata.AddMember(Constants::USER_CHOICES_DIALOG_TYPE_NAME,
                     {sup::dto::UnsignedInteger32Type, dialog_type::kConfirmation});
  std::vector<std::string> options = { ok_text, cancel_text };
  int choice = ui.GetUserChoice(options, metadata);
  return choice == 0 ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
