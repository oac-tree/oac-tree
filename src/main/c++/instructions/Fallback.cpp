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

#include "Fallback.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Fallback::Type = "Fallback";

// Function declaration

// Function definition

ExecutionStatus Fallback::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!HasChildren())
  {
    return ExecutionStatus::SUCCESS;
  }
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
    else
    {
      log_warning("Fallback::ExecuteSingleImpl() - Fallback was ticked again while already successful");
      return child_status;
    }
  }
  return CalculateCompoundStatus();
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

    if (child_status == ExecutionStatus::NOT_STARTED ||
        child_status == ExecutionStatus::NOT_FINISHED)
    {
      return ExecutionStatus::NOT_FINISHED;
    }
    else
    {
      // Forward RUNNING and SUCCESS status of child instruction.
      return child_status;
    }
  }
  return ExecutionStatus::FAILURE;
}

Fallback::Fallback()
  : CompoundInstruction(Type)
{}

Fallback::~Fallback()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
