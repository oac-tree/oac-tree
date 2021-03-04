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
#include "InstructionRegistry.h"
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

const char PATH_DELIMITER='.';

// Function declaration

namespace {

std::pair<std::string, std::string> StripPath(const std::string & path);
bool CloneChildInstructions(Instruction * clone, const Instruction * source);
bool AddClonedChildInstruction(Instruction * instr, const Instruction * child);

} // Unnamed namespace

// Function definition

const Instruction * FindInstruction(const std::vector<const Instruction *> & instructions,
                                    const std::string & name_path)
{
  const Instruction * result = nullptr;
  auto names = StripPath(name_path);
  for (auto inst : instructions)
  {
    if (inst->GetName() == names.first)
    {
      result = inst;
      break;
    }
  }
  if (result == nullptr || names.second.empty())
  {
    log_warning("sup::sequencer::helper::FindInstruction(): could not "
                "find instruction with name_path '%s'", name_path.c_str());
    return result;
  }
  return FindInstruction(result->ChildInstructions(), names.second);
}

Instruction * CloneInstruction(const Instruction * instruction)
{
  if (instruction == nullptr)
  {
    return nullptr;
  }
  auto type = instruction->GetType();
  auto result = GlobalInstructionRegistry().Create(type);
  if (!result)
  {
    return nullptr;
  }
  result->AddAttributes(instruction->GetAttributes());
  CloneChildInstructions(result.get(), instruction);
  return result.release();
}

bool InitialiseVariableAttributes(Instruction & instruction, const AttributeMap & attributes)
{
  bool result = instruction.InitialiseVariableAttributes(attributes);
  for (auto child : instruction.ChildInstructions())
  {
    result = InitialiseVariableAttributes(*child, attributes) && result;
  }
  return result;
}

namespace {

std::pair<std::string, std::string> StripPath(const std::string & path)
{
  std::pair<std::string, std::string> result;
  std::size_t delim_pos = path.find(PATH_DELIMITER);
  result.first = path.substr(0, delim_pos);
  if (delim_pos != std::string::npos)
  {
    result.second = path.substr(delim_pos);
  }
  return result;
}

bool CloneChildInstructions(Instruction * clone, const Instruction * source)
{
  bool result = true;
  if (clone == nullptr || source == nullptr)
  {
    return false;
  }
  for (auto child : source->ChildInstructions())
  {
    result = AddClonedChildInstruction(clone, child) && result;
  }
  return result;
}

bool AddClonedChildInstruction(Instruction * instr, const Instruction * child)
{
  auto compound = dynamic_cast<CompoundInstruction *>(instr);
  if (compound)
  {
    compound->PushBack(CloneInstruction(child));
    return true;
  }
  auto decorator = dynamic_cast<DecoratorInstruction *>(instr);
  if (decorator)
  {
    decorator->SetInstruction(CloneInstruction(child));
    return true;
  }
  return false;
}

} // Unnamed namespace

} // namespace helper

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
