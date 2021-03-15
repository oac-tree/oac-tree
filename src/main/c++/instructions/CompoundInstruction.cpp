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
    instruction->Reset();
  }
}

void CompoundInstruction::HaltImpl()
{
  for (auto instruction : _children)
  {
    instruction->Halt();
  }
}

std::vector<const Instruction *> CompoundInstruction::ChildInstructionsImpl() const
{
  std::vector<const Instruction *> result;
  for (auto instr : _children)
  {
    result.push_back(instr);
  }
  return result;
}

bool CompoundInstruction::SetupImpl(const Procedure & proc)
{
  log_info("CompoundInstruction::Setup - entering function..");
  bool result = true;
  for (auto instruction : _children)
  {
    result = instruction->Setup(proc) && result;
  }
  return result;
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

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC


