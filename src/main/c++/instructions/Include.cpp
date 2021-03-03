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

#include "Include.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Include::Type = "Include";

// Function declaration

// Function definition

ExecutionStatus Include::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  auto child_status = _child->GetStatus();
  if (NeedsExecute(child_status))
  {
    _child->ExecuteSingle(ui, ws);
  }

  return CalculateStatus();
}

bool Include::SetupImpl(const Procedure & proc)
{
  return true;
}

ExecutionStatus Include::CalculateStatus() const
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  return _child->GetStatus();
}

Include::Include()
  : DecoratorInstruction(Type)
{}

Include::~Include()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
