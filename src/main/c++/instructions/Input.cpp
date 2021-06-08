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

#include "Input.h"
#include "UserInterface.h"
#include "Workspace.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Input::Type = "Input";

// Function declaration

// Function definition

ExecutionStatus Input::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  bool status = HasAttribute("output");

  ccs::types::AnyValue value;

  if (status)
  { // Read from workspace
    ws->GetValue(GetAttribute("output"), value);
    status = value.GetSize() > 0;
  }

  if (status)
  { // Read from workspace
    status = ui->GetUserValue(value, GetAttribute("description"));
  }

  if (status)
  { // Write back to workspace
    status = ws->SetValue(GetAttribute("output"), value);
  }

  if (!status)
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

Input::Input()
  : Instruction(Type)
{}

Input::~Input()
{}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
