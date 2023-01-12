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

#include "instruction_helper.h"

#include "compound_instruction.h"
#include "decorator_instruction.h"
#include "include.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction_registry.h>

#include <algorithm>

namespace
{
std::pair<std::string, std::string> StripPath(const std::string& path);
bool CloneChildInstructions(sup::sequencer::Instruction* clone,
                            const sup::sequencer::Instruction* source);
bool AddClonedChildInstruction(sup::sequencer::Instruction* instr,
                               const sup::sequencer::Instruction* child);

}  // Unnamed namespace

namespace sup
{
namespace sequencer
{
namespace InstructionHelper
{
const Instruction* FindInstruction(const std::vector<const Instruction*>& instructions,
                                   const std::string& name_path)
{
  auto names = StripPath(name_path);
  auto it = std::find_if(instructions.begin(), instructions.end(),
                         [&names](const Instruction* instr)
                         {
                           return instr->GetName() == names.first;
                         });
  if (it == instructions.end())
  {
    return nullptr;
  }
  auto result = *it;
  if (names.second.empty())
  {
    return result;
  }
  return FindInstruction(result->ChildInstructions(), names.second);
}

Instruction* CloneInstruction(const Instruction* instruction)
{
  if (instruction == nullptr)
  {
    return nullptr;
  }
  auto type = instruction->GetType();
  if (!GlobalInstructionRegistry().IsRegisteredInstructionName(type))
  {
    return nullptr;
  }
  auto result = GlobalInstructionRegistry().Create(type);
  if (!result)
  {
    return nullptr;
  }
  result->AddAttributes(instruction->GetAttributes());
  if (!CloneChildInstructions(result.get(), instruction))
  {
    return nullptr;
  }
  if (type == Include::Type)
  {
    auto include_result = dynamic_cast<Include*>(result.get());
    auto include_source = dynamic_cast<const Include*>(instruction);
    include_result->SetFilename(include_source->GetFilename());
  }
  return result.release();
}

bool InitialiseVariableAttributes(Instruction& instruction,
                                  const AttributeMap& attributes)
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

using namespace sup::sequencer;

namespace
{
std::pair<std::string, std::string> StripPath(const std::string &path)
{
  std::pair<std::string, std::string> result;
  std::size_t delim_pos = path.find(DefaultSettings::PATH_DELIMITER);
  result.first = path.substr(0, delim_pos);
  if (delim_pos != std::string::npos)
  {
    result.second = path.substr(delim_pos + 1);
  }
  return result;
}

bool CloneChildInstructions(Instruction* clone, const Instruction* source)
{
  for (auto child : source->ChildInstructions())
  {
    if (!AddClonedChildInstruction(clone, child))
    {
      return false;
    }
  }
  return true;
}

bool AddClonedChildInstruction(Instruction* instr, const Instruction* child)
{
  auto cloned_child = InstructionHelper::CloneInstruction(child);
  if (cloned_child == nullptr)
  {
    return false;
  }
  auto compound = dynamic_cast<CompoundInstruction*>(instr);
  if (compound)
  {
    compound->PushBack(cloned_child);
    return true;
  }
  auto decorator = dynamic_cast<DecoratorInstruction*>(instr);
  if (decorator)
  {
    decorator->SetInstruction(cloned_child);
    return true;
  }
  return false;
}

}  // Unnamed namespace
