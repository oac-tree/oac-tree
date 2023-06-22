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
  AddAttributeDefinition(DESCRIPTION_ATTRIBUTE, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(OK_ATTRIBUTE, sup::dto::StringType);
  AddAttributeDefinition(CANCEL_ATTRIBUTE, sup::dto::StringType);
}

UserConfirmation::~UserConfirmation() = default;

ExecutionStatus UserConfirmation::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ws;
  auto description = GetAttributeValue<std::string>(DESCRIPTION_ATTRIBUTE);
  std::string ok_text =
      HasAttribute(OK_ATTRIBUTE) ? GetAttributeValue<std::string>(OK_ATTRIBUTE) : DEFAULT_OK_TEXT;
  std::string cancel_text = HasAttribute(CANCEL_ATTRIBUTE)
                                ? GetAttributeValue<std::string>(CANCEL_ATTRIBUTE)
                                : DEFAULT_CANCEL_TEXT;
  auto metadata = CreateUserChoiceMetadata();
  metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, description);
  metadata.AddMember(Constants::USER_CHOICES_DIALOG_TYPE_NAME,
                     {sup::dto::UnsignedInteger32Type, dialog_type::kConfirmation});
  auto options = CreateIndexedOptions( { ok_text, cancel_text });
  int choice = ui.GetUserChoice(options, metadata);
  return choice == 0 ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
