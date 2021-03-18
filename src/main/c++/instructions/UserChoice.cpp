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
* Copyright (c) : 2010-2020 ITER Organization,
*                 CS 90 046
*                 13067 St. Paul-lez-Durance Cedex
*                 France
*
* This file is part of ITER CODAC software.
* For the terms and conditions of redistribution or use of this software
* refer to the file ITER-LICENSE.TXT located in the top level directory
* of the distribution package.
******************************************************************************/

// Global header files

#include <common/log-api.h>

// Local header files

#include "UserChoice.h"
#include "UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string UserChoice::Type = "UserChoice";

static const std::string DESCRIPTION_ATTRIBUTE = "description";

// Function declaration

// Function definition

ExecutionStatus UserChoice::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
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
    log_warning("UserChoice::ExecuteSingleImpl() - invalid choice");
    return ExecutionStatus::FAILURE;
  }
  auto selected = ChildInstructions()[choice];
  auto selected_status = selected->GetStatus();
  if (NeedsExecute(selected_status))
  {
    selected->ExecuteSingle(ui, ws);
    return selected->GetStatus();
  }
  log_warning("UserChoice::ExecuteSingleImpl() - Selected instruction was not in the correct "
              "state to execute: '%s'", StatusToString(selected_status).c_str());
  return ExecutionStatus::FAILURE;
}

std::vector<std::string> UserChoice::GetChoices() const
{
  std::vector<std::string> result;
  for (auto instruction : ChildInstructions())
  {
    std::string description = instruction->GetName() + std::string(" (type:") +
                              instruction->GetType() + std::string(")");
    result.push_back(description);
  }
  return result;
}

UserChoice::UserChoice()
  : CompoundInstruction(Type)
{}

UserChoice::~UserChoice()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
