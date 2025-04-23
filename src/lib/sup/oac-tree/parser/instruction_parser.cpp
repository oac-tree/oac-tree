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

#include "instruction_parser.h"

#include <sup/oac-tree/compound_instruction.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/decorator_instruction.h>
#include <sup/oac-tree/instructions/include.h>
#include <sup/oac-tree/instruction_registry.h>

#include <sup/oac-tree/exceptions.h>

namespace sup
{
namespace oac_tree
{
static void AddChildInstructions(Instruction *instruction,
                                 const std::vector<sup::xml::TreeData> &children);
static std::string GetNameAttribute(const sup::xml::TreeData& data);

std::unique_ptr<Instruction> ParseInstruction(const sup::xml::TreeData &data)
{
  auto instr_type = data.GetNodeName();
  if (!GlobalInstructionRegistry().IsRegisteredInstructionName(instr_type))
  {
    std::string error_message = "ParseInstruction(): trying to create unregistered "
                                "instruction with typename [" + instr_type + "]";
    throw ParseException(error_message);
  }
  auto instr = GlobalInstructionRegistry().Create(instr_type);
  if (!instr)
  {
    std::string error_message = "ParseInstruction(): could not create instruction "
                                "with typename [" + instr_type + "]";
    throw ParseException(error_message);
  }
  for (auto& [attr_name, attr_value] : data.Attributes())
  {
    instr->AddAttribute(attr_name, attr_value);
  }
  AddChildInstructions(instr.get(), data.Children());
  return instr;
}

static void AddChildInstructions(Instruction *instruction,
                                 const std::vector<sup::xml::TreeData> &children)
{
  for (auto &child : children)
  {
    auto child_instr = ParseInstruction(child);
    if (!child_instr)
    {
      std::string error_message =
        "AddChildInstructions(): could not create instruction from TreeData "
        "of type: <" + child.GetNodeName() + "> and name [" + GetNameAttribute(child) + "]";
      throw ParseException(error_message);
    }
    if (!AppendChildInstruction(*instruction, std::move(child_instr)))
    {
      std::string error_message =
        "AddChildInstructions(): wrong amount of child instructions for leaf "
        "instruction of type: [" + instruction->GetType() +
        "] and name [" + instruction->GetName() + "]";
      throw ParseException(error_message);
    }
  }
}

static std::string GetNameAttribute(const sup::xml::TreeData& data)
{
  if (!data.HasAttribute(Constants::NAME_ATTRIBUTE_NAME))
  {
    return {};
  }
  return data.GetAttribute(Constants::NAME_ATTRIBUTE_NAME);
}

}  // namespace oac_tree

}  // namespace sup
