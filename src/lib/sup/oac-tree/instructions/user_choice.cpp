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

#include "user_choice.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>

namespace sup
{
namespace oac_tree
{
const std::string UserChoice::Type = "UserChoice";

UserChoice::UserChoice()
  : CompoundInstruction(UserChoice::Type)
  , m_choice{-1}
  , m_future{}
{
  AddAttributeDefinition(Constants::DESCRIPTION_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kBoth);
}

UserChoice::~UserChoice() = default;

bool UserChoice::InitHook(UserInterface& ui, Workspace& ws)
{
  std::string main_text;
  if (!GetAttributeValueAs(Constants::DESCRIPTION_ATTRIBUTE_NAME, ws, ui, main_text))
  {
    return false;
  }
  auto metadata = CreateUserChoiceMetadata();
  metadata.AddMember(Constants::USER_CHOICES_TEXT_NAME, main_text);
  metadata.AddMember(Constants::USER_CHOICES_DIALOG_TYPE_NAME,
                     {sup::dto::UnsignedInteger32Type, dialog_type::kSelection});
  auto options = GetChoices();
  m_future = CreateUserChoiceFuture(ui, *this, options, metadata);
  if (!m_future)
  {
    return false;
  }
  return true;
}

ExecutionStatus UserChoice::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (IsHaltRequested() || !m_future)
  {
    return ExecutionStatus::FAILURE;
  }
  if (m_choice < 0)
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
    if (choice < 0 || choice >= ChildrenCount())
    {
      std::string warning_message = InstructionWarningProlog(*this) +
        "user choice [" + std::to_string(choice) + "] is not a valid value for [" +
        std::to_string(ChildrenCount()) + "] child instructions";
      LogWarning(ui, warning_message);
      return ExecutionStatus::FAILURE;
    }
    m_choice = choice;
    return ExecutionStatus::NOT_FINISHED;  // TODO: remove this if we no longer need this...
  }
  auto selected = ChildInstructions()[m_choice];
  selected->ExecuteSingle(ui, ws);
  return selected->GetStatus();
}

void UserChoice::ResetHook(UserInterface& ui)
{
  m_choice = -1;
  m_future.reset();
  ResetChildren(ui);
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

std::vector<std::string> UserChoice::GetChoices() const
{
  std::vector<std::string> option_names;
  for (auto instruction : ChildInstructions())
  {
    std::string description = instruction->GetName() + " (type:" + instruction->GetType() + ")";
    option_names.push_back(description);
  }
  return option_names;
}

}  // namespace oac_tree

}  // namespace sup
