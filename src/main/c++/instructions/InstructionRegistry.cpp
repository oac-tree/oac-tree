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

#include "InstructionRegistry.h"
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

InstructionRegistry & InstructionRegistry::Instance()
{
  static InstructionRegistry instance;
  return instance;
}

bool InstructionRegistry::RegisterInstruction(std::string name, InstructionConstructor constructor)
{
  _instruction_map[name] = constructor;
  return true;
}

std::unique_ptr<Instruction> InstructionRegistry::Create(std::string name)
{
  return std::unique_ptr<Instruction>(_instruction_map[name]());
}

std::vector<std::string> InstructionRegistry::RegisteredInstructionNames() const
{
  std::vector<std::string> result;
  for (const auto elem : _instruction_map)
  {
    result.push_back(elem.first);
  }
  return result;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
