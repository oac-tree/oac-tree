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

#include "UserInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

void UserInterface::StartSingleStepImpl()
{}

void UserInterface::EndSingleStepImpl()
{}

void UserInterface::UpdateInstructionStatus(const Instruction * instruction)
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  UpdateInstructionStatusImpl(instruction);
}

void UserInterface::StartSingleStep()
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  StartSingleStepImpl();
}

void UserInterface::EndSingleStep()
{
  std::lock_guard<std::mutex> lock(_ui_mutex);
  EndSingleStepImpl();
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
