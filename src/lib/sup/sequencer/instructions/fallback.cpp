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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "fallback.h"

namespace sup
{
namespace sequencer
{
const std::string Fallback::Type = "Fallback";

Fallback::Fallback()
  : CompoundInstruction(Fallback::Type)
{}

Fallback::~Fallback() = default;

ExecutionStatus Fallback::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::FAILURE)
    {
      continue;
    }
    if (NeedsExecute(child_status))
    {
      instruction->ExecuteSingle(ui, ws);
      break;
    }
  }
  return CalculateCompoundStatus();
}

std::vector<const Instruction*> Fallback::NextInstructionsImpl() const
{
  std::vector<const Instruction*> result;
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::FAILURE)
    {
      continue;
    }
    if (ReadyForExecute(child_status))
    {
      result.push_back(instruction);
      return result;
    }
    // There is no next when a child is encountered that has status SUCCESS/RUNNING:
    break;
  }
  return result;
}

ExecutionStatus Fallback::CalculateCompoundStatus() const
{
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::FAILURE)
    {
      continue;
    }
    if (child_status == ExecutionStatus::NOT_STARTED
        || child_status == ExecutionStatus::NOT_FINISHED)
    {
      return ExecutionStatus::NOT_FINISHED;
    }
    return child_status;
  }
  return ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
