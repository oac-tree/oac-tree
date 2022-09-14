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

#include "sequence.h"

#include <sup/sequencer/log.h>

namespace sup
{
namespace sequencer
{
const std::string Sequence::Type = "Sequence";

ExecutionStatus Sequence::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  if (!HasChildren())
  {
    return ExecutionStatus::SUCCESS;
  }
  for (auto instruction : ChildInstructions())
  {
    auto child_status = instruction->GetStatus();

    if (child_status == ExecutionStatus::SUCCESS)
    {
      continue;
    }

    if (NeedsExecute(child_status))
    {
      instruction->ExecuteSingle(ui, ws);
      break;
    }
    else
    {
      log::Warning("Sequence::ExecuteSingleImpl() - Sequence was ticked again while already failed");
      return child_status;
    }
  }
  return CalculateCompoundStatus();
}

ExecutionStatus Sequence::CalculateCompoundStatus() const
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
    else
    {
      // Forward RUNNING and FAILURE status of child instruction.
      return child_status;
    }
  }
  return ExecutionStatus::SUCCESS;
}

Sequence::Sequence() : CompoundInstruction(Sequence::Type) {}

Sequence::~Sequence() = default;

}  // namespace sequencer

}  // namespace sup
