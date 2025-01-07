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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <sup/sequencer/compound_instruction.h>
#include <sup/sequencer/decorator_instruction.h>
#include "include.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction_registry.h>

#include <algorithm>

namespace
{
std::pair<std::string, std::string> StripPath(const std::string& path);
bool CloneChildInstructions(sup::sequencer::Instruction& clone,
                            const sup::sequencer::Instruction* source);
bool AddClonedChildInstruction(sup::sequencer::Instruction& instr,
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

std::unique_ptr<Instruction> CloneInstruction(const Instruction* instruction)
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
  result->AddAttributes(instruction->GetStringAttributes());
  if (!CloneChildInstructions(*result, instruction))
  {
    return nullptr;
  }
  return std::move(result);
}

bool InitialisePlaceholderAttributes(Instruction& instruction,
                                  const StringAttributeList& source_attributes)
{
  bool result = instruction.InitialisePlaceholderAttributes(source_attributes);
  for (auto child : instruction.ChildInstructions())
  {
    result = InitialisePlaceholderAttributes(*child, source_attributes) && result;
  }
  return result;
}

bool InitialisePlaceholderAttributes(AttributeHandler& attribute_handler,
                                  const StringAttributeList& source_attributes)
{
  bool result = true;
  for (auto& attr : attribute_handler.GetStringAttributes())
  {
    auto attr_value = attr.second;
    if (AttributeStartsWith(attr_value, DefaultSettings::PLACEHOLDER_ATTRIBUTE_CHAR))
    {
      auto var_name = attr_value.substr(1);
      auto it = FindStringAttribute(source_attributes, var_name);
      if (it == source_attributes.end())
      {
        result = false;
        continue;
      }
      auto var_value = it->second;
      attribute_handler.SetStringAttribute(attr.first, var_value);
    }
  }
  return result;
}

bool AttributeStartsWith(const std::string& attr_str, char c)
{
  if (attr_str.empty())
  {
    return false;
  }
  return attr_str[0] == c;
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

bool CloneChildInstructions(Instruction& clone, const Instruction* source)
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

bool AddClonedChildInstruction(Instruction& instr, const Instruction* child)
{
  auto cloned_child = InstructionHelper::CloneInstruction(child);
  if (!cloned_child)
  {
    return false;
  }
  return AppendChildInstruction(instr, std::move(cloned_child));
}

}  // Unnamed namespace
