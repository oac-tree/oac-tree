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

#include "InstructionHelper.h"
#include "CompoundInstruction.h"
#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

namespace helper {

// Global variables

// Function declaration

// Function definition

const Instruction * FindInstruction(const std::vector<const Instruction *> & instructions,
                                    const std::string & name_path)
{
  return nullptr;
}

std::vector<const Instruction *> GetChildInstructions(const Instruction * instruction)
{
  auto compound = dynamic_cast<const CompoundInstruction *>(instruction);
  if (compound)
  {
    return compound->ChildInstructions();
  }

  std::vector<const Instruction *> result;
  auto decorator = dynamic_cast<const DecoratorInstruction *>(instruction);
  if (decorator)
  {
    result.push_back(decorator->ChildInstruction());
  }
  return result;
}

Instruction * CloneInstruction(const Instruction * instruction)
{
  return nullptr;
}

bool InitialiseVariableAttributes(Instruction & instruction, const AttributeMap * attributes)
{
  return false;
}

} // namespace helper

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
