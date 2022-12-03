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
#include <sup/sequencer/log.h>
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
  if (choice < 0 || choice >= ChildInstructions().size())
  {
    log::Warning("UserChoice::ExecuteSingleImpl() - invalid choice");
    return ExecutionStatus::FAILURE;
  }
  auto selected = ChildInstructions()[choice];
  auto selected_status = selected->GetStatus();
  if (NeedsExecute(selected_status))
  {
    selected->ExecuteSingle(ui, ws);
    return selected->GetStatus();
  }
  log::Warning(
      "UserChoice::ExecuteSingleImpl() - Selected instruction was not in the correct "
      "state to execute: '%s'",
      StatusToString(selected_status).c_str());
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
