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

#include <sup/sequencer/constants.h>
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
  , m_choice{-1}
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
  // Negative number for choice member variable indicates choice was not made yet
  if (m_choice < 0)
  {
    std::string main_text =
      HasAttribute(DESCRIPTION_ATTRIBUTE) ? GetAttributeValue<std::string>(DESCRIPTION_ATTRIBUTE)
                                          : "";
    auto metadata = CreateUserChoiceMetadata();
    metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, main_text);
    auto options = GetChoices();
    int choice = ui.GetUserChoice(options, metadata);
    if (choice < 0 || choice >= ChildrenCount())
    {
      std::string warning_message = InstructionWarningProlog(*this) +
        "user choice [" + std::to_string(choice) + "] is not a valid value for [" +
        std::to_string(ChildrenCount()) + "] child instructions";
      ui.LogWarning(warning_message);
      return ExecutionStatus::FAILURE;
    }
    m_choice = choice;
    // Request second tick so NextInstructions member function can correctly give the chosen
    // child instruction:
    return ExecutionStatus::NOT_FINISHED;
  }
  auto selected = ChildInstructions()[m_choice];
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

void UserChoice::ResetHook()
{
  m_choice = -1;
  ResetChildren();
}

std::vector<const Instruction*> UserChoice::NextInstructionsImpl() const
{
  std::vector<const Instruction*> result;
  if (m_choice < 0 || m_choice >= ChildrenCount())
  {
    return result;
  }
  auto instruction = ChildInstructions()[m_choice];
  if (ReadyForExecute(instruction->GetStatus()))
  {
    result.push_back(instruction);
  }
  return result;
}

std::vector<std::pair<std::string, int>> UserChoice::GetChoices() const
{
  std::vector<std::pair<std::string, int>> result;
  int index = 0;
  for (auto instruction : ChildInstructions())
  {
    std::string description = instruction->GetName() + " (type:" + instruction->GetType() + ")";
    result.emplace_back(description, index++);
  }
  return result;
}

}  // namespace sequencer

}  // namespace sup
