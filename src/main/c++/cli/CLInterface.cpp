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

#include <iostream>

#include <common/log-api.h>

// Local header files

#include "CLInterface.h"
#include "Instruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

CLInterface::CLInterface(bool verbose)
  : _verbose{verbose}
{}

CLInterface::~CLInterface() = default;

void CLInterface::UpdateInstructionStatus(const Instruction * instruction)
{
  auto instruction_type = instruction->GetType();
  auto instruction_name = instruction->GetName();
  auto status = instruction->GetStatus();
  auto status_str=StatusToString(status);

  log_info("CLInterface::Instruction %s - %s: %s", instruction_type.c_str(), instruction_name.c_str(), status_str.c_str());
}

void CLInterface::StartSingleStep()
{
  if (_verbose)
  {
    log_info("CLInterface::Start single execution step");
  }
}

void CLInterface::EndSingleStep()
{
  if (_verbose)
  {
    log_info("CLInterface::End single execution step");
  }
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
