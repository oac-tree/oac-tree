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

#include "Inverter.h"
#include "InstructionRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

ExecutionStatus Inverter::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  auto child_status = _child->GetStatus();
  if (child_status == ExecutionStatus::UNDEFINED ||
    child_status == ExecutionStatus::STARTED)
  {
    _child->ExecuteSingle(ui, ws);
  }

  return CalculateStatus();
}

ExecutionStatus Inverter::CalculateStatus() const
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  auto child_status = _child->GetStatus();
  auto status = child_status;

  switch (child_status)
  {
  case ExecutionStatus::FAILURE:
    status = ExecutionStatus::SUCCESS;
    break;
  case ExecutionStatus::SUCCESS:
    status = ExecutionStatus::FAILURE;
    break;
  default:
    break;
  }
  return status;
}

Inverter::Inverter()
  : DecoratorInstruction(InstructionRegistry::InverterType)
{}

Inverter::~Inverter()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
