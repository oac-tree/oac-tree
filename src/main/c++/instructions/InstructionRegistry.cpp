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
#include "Inverter.h"
#include "Sequence.h"
#include "SuccessNode.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

InstructionRegistry & GlobalInstructionRegistry()
{
  static InstructionRegistry global_instruction_registry;
  return global_instruction_registry;
}

bool InstructionRegistry::RegisterInstruction(std::string name, InstructionConstructor constructor)
{
  _instruction_map[name] = constructor;
  return true;
}

std::unique_ptr<Instruction> InstructionRegistry::Create(std::string name)
{
  auto entry = _instruction_map.find(name);
  if (entry == _instruction_map.end())
  {
    log_error("InstructionRegistry::Create('%s') - Instruction not registered", name.c_str());
    return {};
  }
  log_info("InstructionRegistry::Create('%s') - Create instruction ..", name.c_str());
  return std::unique_ptr<Instruction>(entry->second());
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

bool InitGlobalInstructionRegistry()
{
  bool result = true;
  result = RegisterInstruction<Inverter>() && result;
  result = RegisterInstruction<Sequence>() && result;
  result = RegisterInstruction<SuccessNode>() && result;

  return result;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
