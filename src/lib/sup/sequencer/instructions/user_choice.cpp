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

#include "user_choice.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
{
const std::string UserChoice::Type = "UserChoice";

static const std::string DESCRIPTION_ATTRIBUTE = "description";

UserChoice::UserChoice()
  : CompoundInstruction(UserChoice::Type)
{
  AddAttributeDefinition(DESCRIPTION_ATTRIBUTE, sup::dto::StringType);
}

UserChoice::~UserChoice() = default;

ExecutionStatus UserChoice::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ws;

  if (!HasChildren())
  {
    return ExecutionStatus::SUCCESS;
  }
  std::string description =
    HasAttribute(DESCRIPTION_ATTRIBUTE) ? GetAttributeValue<std::string>(DESCRIPTION_ATTRIBUTE)
                                        : "";

  auto options = GetChoices();
  int choice = ui.GetUserChoice(options, description);
  if (choice < 0 || choice >= ChildrenCount())
  {
    std::string warning_message = InstructionWarningProlog(*this) +
      "user choice [" + std::to_string(choice) + "] is not a valid value for [" +
      std::to_string(ChildrenCount()) + "] child instructions";
    ui.LogWarning(warning_message);
    return ExecutionStatus::FAILURE;
  }
  auto selected = ChildInstructions()[choice];
  auto selected_status = selected->GetStatus();
  if (NeedsExecute(selected_status))
  {
    selected->ExecuteSingle(ui, ws);
    return selected->GetStatus();
  }
  std::string error_message = InstructionErrorProlog(*this) +
    "child instruction of type [" + selected->GetType() + "] was already finished with status [" +
    StatusToString(selected_status) + "]";
  ui.LogError(error_message);
  return ExecutionStatus::FAILURE;
}

std::vector<std::string> UserChoice::GetChoices() const
{
  std::vector<std::string> result;
  for (auto instruction : ChildInstructions())
  {
    std::string description = instruction->GetName() + " (type:" + instruction->GetType() + ")";
    result.push_back(description);
  }
  return result;
}

}  // namespace sequencer

}  // namespace sup
