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

#include "CompoundInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

void CompoundInstruction::ResetHook()
{
  for (auto instruction : _children)
  {
    instruction->ResetStatus();
  }
}

CompoundInstruction::CompoundInstruction(const std::string & type)
    : Instruction(type)
{}

CompoundInstruction::~CompoundInstruction()
{
  for (auto instruction : _children)
  {
    delete instruction;
  }
}

void CompoundInstruction::PushBack(Instruction * instruction)
{
  _children.push_back(instruction);
}

bool CompoundInstruction::Setup(Workspace * ws){
    bool ret=true;
    for (auto instruction : _children)
    {
      auto myName=GetName();
      auto myType=GetType();
      auto childType=instruction->GetType();
      auto childName=instruction->GetName();
      log_info("CompoundInstruction::Setup - %s:%s Setup of %s: %s", myType.c_str(), myName.c_str(), childType.c_str(), childName.c_str());
      ret&=instruction->Setup(ws);
    }
    return ret;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC


