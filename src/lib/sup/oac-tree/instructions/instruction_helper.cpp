/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "instruction_helper.h"

#include <sup/oac-tree/compound_instruction.h>
#include <sup/oac-tree/decorator_instruction.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction_registry.h>

#include <algorithm>

namespace
{
std::pair<std::string, std::string> StripPath(const std::string& path);
bool CloneChildInstructions(sup::oac_tree::Instruction& clone,
                            const sup::oac_tree::Instruction* source);
bool AddClonedChildInstruction(sup::oac_tree::Instruction& instr,
                               const sup::oac_tree::Instruction* child);
}  // Unnamed namespace

namespace sup
{
namespace oac_tree
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
  auto [_, instruction] = names;
  if (it == instructions.end())
  {
    return nullptr;
  }
  auto result = *it;
  if (instruction.empty())
  {
    return result;
  }
  return FindInstruction(result->ChildInstructions(), instruction);
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
  for (auto& [attr_name, attr_value] : attribute_handler.GetStringAttributes())
  {
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
      attribute_handler.SetStringAttribute(attr_name, var_value);
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

}  // namespace oac_tree

}  // namespace sup

using namespace sup::oac_tree;

namespace
{
std::pair<std::string, std::string> StripPath(const std::string &path)
{
  std::size_t delim_pos = path.find(DefaultSettings::PATH_DELIMITER);
  std::string prefix = path.substr(0, delim_pos);
  std::string sufix;
  if (delim_pos != std::string::npos)
  {
    sufix = path.substr(delim_pos + 1);
  }
  return {prefix, sufix};
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
