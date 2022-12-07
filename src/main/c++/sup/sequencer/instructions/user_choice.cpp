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
 * Copyright (c) : 2010-2022 ITER Organization,
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
#include <sup/sequencer/log_severity.h>
#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
{
const std::string UserChoice::Type = "UserChoice";

static const std::string DESCRIPTION_ATTRIBUTE = "description";

UserChoice::UserChoice()
  : CompoundInstruction(UserChoice::Type)
{}

UserChoice::~UserChoice() = default;

ExecutionStatus UserChoice::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  (void)ws;

  if (!HasChildren())
  {
    return ExecutionStatus::SUCCESS;
  }
  std::string description;
  if (HasAttribute(DESCRIPTION_ATTRIBUTE))
  {
    description = GetAttribute(DESCRIPTION_ATTRIBUTE);
  }

  auto options = GetChoices();
  int choice = ui->GetUserChoice(options, description);
  if (choice < 0 || choice >= ChildrenCount())
  {
    std::string warning_message =
      "sup::sequencer::UserChoice::ExecuteSingleImpl(): user choice [" + std::to_string(choice) +
      "] is not a valid value for [" + std::to_string(ChildrenCount()) + "] child instructions";
    ui->Log(log::SUP_SEQ_LOG_WARNING, warning_message);
    return ExecutionStatus::FAILURE;
  }
  auto selected = ChildInstructions()[choice];
  auto selected_status = selected->GetStatus();
  if (NeedsExecute(selected_status))
  {
    selected->ExecuteSingle(ui, ws);
    return selected->GetStatus();
  }
  std::string error_message =
    "sup::sequencer::UserChoice::ExecuteSingleImpl(): child instruction of type [" +
    selected->GetType() + "] was already finished with status [" +
    StatusToString(selected_status) + "]";
  ui->Log(log::SUP_SEQ_LOG_ERR, error_message);
  return ExecutionStatus::FAILURE;
}

std::vector<std::string> UserChoice::GetChoices() const
{
  std::vector<std::string> result;
  for (auto instruction : ChildInstructions())
  {
    std::string description =
        instruction->GetName() + std::string(" (type:") + instruction->GetType() + std::string(")");
    result.push_back(description);
  }
  return result;
}

}  // namespace sequencer

}  // namespace sup
