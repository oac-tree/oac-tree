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

#include "reactive_sequence.h"

namespace sup
{
namespace oac_tree
{
const std::string ReactiveSequence::Type = "ReactiveSequence";

ReactiveSequence::ReactiveSequence()
  : CompoundInstruction(ReactiveSequence::Type)
{}

ReactiveSequence::~ReactiveSequence() = default;

ExecutionStatus ReactiveSequence::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::SUCCESS)
    {
      continue;
    }
    // child FAILED is not taken into account here, as the ReactiveSequence should not have been
    // ticked then.
    if (NeedsExecute(child_status))
    {
      instruction->ExecuteSingle(ui, ws);
      if (instruction->GetStatus() == ExecutionStatus::RUNNING)
      {
        // Reset all other children and immediately return RUNNING:
        ResetOtherChildren(instruction, ui);
        return ExecutionStatus::RUNNING;
      }
      break;
    }
  }
  return CalculateCompoundStatus();
}

std::vector<const Instruction*> ReactiveSequence::NextInstructionsImpl() const
{
  std::vector<const Instruction*> result;
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::SUCCESS)
    {
      continue;
    }
    if (ReadyForExecute(child_status))
    {
      result.push_back(instruction);
      return result;
    }
    // There is no next when a child is encountered that has status FAILURE/RUNNING:
    break;
  }
  return result;
}

ExecutionStatus ReactiveSequence::CalculateCompoundStatus() const
{
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();
    if (child_status == ExecutionStatus::SUCCESS)
    {
      continue;
    }
    if (child_status == ExecutionStatus::NOT_STARTED
        || child_status == ExecutionStatus::NOT_FINISHED)
    {
      return ExecutionStatus::NOT_FINISHED;
    }
    return child_status;  // FAILURE or RUNNING
  }
  return ExecutionStatus::SUCCESS;
}

void ReactiveSequence::ResetOtherChildren(const Instruction* instr, UserInterface& ui)
{
  for (auto other_instruction : ChildInstructions())
  {
    if (other_instruction != instr)
    {
      other_instruction->Reset(ui);
    }
  }
}

}  // namespace oac_tree

}  // namespace sup
