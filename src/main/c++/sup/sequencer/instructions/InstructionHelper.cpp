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
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <sup/sequencer/log.h>

// Local header files

#include "CompoundInstruction.h"
#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction_registry.h>

#include "DecoratorInstruction.h"
#include "Include.h"
#include "InstructionHelper.h"

// Type definition

// Global variables

// Function declaration

namespace
{
std::pair<std::string, std::string> StripPath(const std::string &path);
bool CloneChildInstructions(sup::sequencer::Instruction *clone,
                            const sup::sequencer::Instruction *source);
bool AddClonedChildInstruction(sup::sequencer::Instruction *instr,
                               const sup::sequencer::Instruction *child);

}  // Unnamed namespace

namespace sup
{
namespace sequencer
{
namespace InstructionHelper
{
// Function definition

const Instruction *FindInstruction(const std::vector<const Instruction *> &instructions,
                                   const std::string &name_path)
{
  const Instruction *result = nullptr;
  auto names = StripPath(name_path);
  log::Debug(
      "sup::sequencer::InstructionHelper::FindInstruction(): checking "
      "names ('%s','%s')..",
      names.first.c_str(), names.second.c_str());
  for (auto inst : instructions)
  {
    if (inst->GetName() == names.first)
    {
      result = inst;
      break;
    }
  }
  if (result == nullptr)
  {
    log::Warning(
        "sup::sequencer::InstructionHelper::FindInstruction(): could not "
        "find instruction with name_path '%s'",
        name_path.c_str());
    return result;
  }
  if (names.second.empty())
  {
    return result;
  }
  return FindInstruction(result->ChildInstructions(), names.second);
}

Instruction *CloneInstruction(const Instruction *instruction)
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
  auto include = dynamic_cast<Include *>(result.get());
  if (include)
  {
    auto other = dynamic_cast<const Include *>(instruction);
    if (other)
    {
      include->SetFilename(other->GetFilename());
    }
  }
  result->AddAttributes(instruction->GetAttributes());
  CloneChildInstructions(result.get(), instruction);
  return result.release();
}

bool InitialiseVariableAttributes(Instruction &instruction, const AttributeMap &attributes)
{
  bool result = instruction.InitialiseVariableAttributes(attributes);
  for (auto child : instruction.ChildInstructions())
  {
    result = InitialiseVariableAttributes(*child, attributes) && result;
  }
  return result;
}

}  // namespace InstructionHelper

}  // namespace sequencer

}  // namespace sup

namespace
{
std::pair<std::string, std::string> StripPath(const std::string &path)
{
  std::pair<std::string, std::string> result;
  std::size_t delim_pos = path.find(sup::sequencer::DefaultSettings::PATH_DELIMITER);
  result.first = path.substr(0, delim_pos);
  if (delim_pos != std::string::npos)
  {
    result.second = path.substr(delim_pos + 1);
  }
  return result;
}

bool CloneChildInstructions(sup::sequencer::Instruction *clone,
                            const sup::sequencer::Instruction *source)
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

bool AddClonedChildInstruction(sup::sequencer::Instruction *instr,
                               const sup::sequencer::Instruction *child)
{
  auto compound = dynamic_cast<sup::sequencer::CompoundInstruction *>(instr);
  if (compound)
  {
    compound->PushBack(sup::sequencer::InstructionHelper::CloneInstruction(child));
    return true;
  }
  auto decorator = dynamic_cast<sup::sequencer::DecoratorInstruction *>(instr);
  if (decorator)
  {
    decorator->SetInstruction(sup::sequencer::InstructionHelper::CloneInstruction(child));
    return true;
  }
  return false;
}

}  // Unnamed namespace

extern "C"
{
  // C API function definitions

}  // extern C
