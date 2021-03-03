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

#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

void DecoratorInstruction::ResetHook()
{
  if (_child)
  {
    _child->ResetStatus();
  }
}

bool DecoratorInstruction::SetupImpl(const Procedure & proc){
    return _child->Setup(proc);
}

DecoratorInstruction::DecoratorInstruction(const std::string & type)
  : Instruction(type)
  , _child{}
{}

DecoratorInstruction::~DecoratorInstruction()
{}

const Instruction * DecoratorInstruction::ChildInstruction() const
{
  return _child.get();
}

void DecoratorInstruction::SetInstruction(Instruction * instruction)
{
  _child.reset(instruction);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
