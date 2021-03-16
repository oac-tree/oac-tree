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

void CLInterface::UpdateInstructionStatusImpl(const Instruction * instruction)
{
  auto instruction_type = instruction->GetType();
  auto instruction_name = instruction->GetName();
  auto status = instruction->GetStatus();

  std::cout << "Instruction: (" << instruction_type << ":" << instruction_name << ") : ";
  std::cout << StatusToString(status) << std::endl;
}

void CLInterface::StartSingleStepImpl()
{
  if (_verbose)
  {
    std::cout << "Start single execution step" << std::endl;
  }
}

void CLInterface::EndSingleStepImpl()
{
  if (_verbose)
  {
    std::cout << "End single execution step" << std::endl;
  }
}

CLInterface::CLInterface(bool verbose)
  : _verbose{verbose}
{}

CLInterface::~CLInterface() = default;

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
