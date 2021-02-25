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
  auto status = instruction->GetStatus();

  std::cout << "Instruction: " << instruction_type << ": ";
  std::cout << StatusToString(status) << std::endl;
}

void CLInterface::StartSingleStep()
{
  if (_verbose)
  {
    std::cout << "Start single execution step" << std::endl;
  }
}

void CLInterface::EndSingleStep()
{
  if (_verbose)
  {
    std::cout << "End single execution step" << std::endl;
  }
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
