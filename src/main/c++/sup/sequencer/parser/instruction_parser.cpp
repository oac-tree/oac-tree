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

#include "instruction_parser.h"

#include <sup/sequencer/instructions/compound_instruction.h>
#include <sup/sequencer/instructions/decorator_instruction.h>
#include <sup/sequencer/instructions/include.h>
#include <sup/sequencer/instruction_registry.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/log.h>

namespace sup
{
namespace sequencer
{
static void AddChildInstructions(Instruction *instruction, const std::vector<TreeData> &children,
                                 const std::string &filename);
static void AddChildrenToDecorator(DecoratorInstruction *decorator,
                                   const std::vector<TreeData> &children,
                                   const std::string &filename);
static void AddChildrenToCompound(CompoundInstruction *compound,
                                  const std::vector<TreeData> &children,
                                  const std::string &filename);

std::unique_ptr<Instruction> ParseInstruction(const TreeData &data, const std::string &filename)
{
  auto instr_type = data.GetType();
  auto instr = GlobalInstructionRegistry().Create(instr_type);
  if (!instr)
  {
    std::string error_message = "sup::sequencer::ParseInstruction(): could not create instruction "
                                "with typename: " + instr_type;
    throw ParseException(error_message);
  }
  for (auto &attr : data.Attributes())
  {
    instr->AddAttribute(attr.first, attr.second);
  }
  AddChildInstructions(instr.get(), data.Children(), filename);
  return instr;
}

static void AddChildInstructions(Instruction *instruction, const std::vector<TreeData> &children,
                                 const std::string &filename)
{
  auto instr_name = instruction->GetName();
  auto instr_type = instruction->GetType();

  // Only set source directory for Include instruction:
  auto include = dynamic_cast<Include *>(instruction);
  if (include)
  {
    include->SetFilename(filename);
    return;
  }

  auto decorator = dynamic_cast<DecoratorInstruction *>(instruction);
  if (decorator)
  {
    AddChildrenToDecorator(decorator, children, filename);
    return;
  }

  auto compound = dynamic_cast<CompoundInstruction *>(instruction);
  if (compound)
  {
    AddChildrenToCompound(compound, children, filename);
    return;
  }
  if (!children.empty())
  {
    std::string error_message =
      "sup::sequencer::AddChildInstructions(): non-zero amount of child instructions for leaf "
      "instruction of type: [" + instruction->GetType() +
      "] and name [" + instruction->GetName() + "]";
    throw ParseException(error_message);
  }
}

static void AddChildrenToDecorator(DecoratorInstruction *decorator,
                                   const std::vector<TreeData> &children,
                                   const std::string &filename)
{
  if (children.size() != 1u)
  {
    std::string error_message =
      "sup::sequencer::AddChildrenToDecorator(): number of children not equal to 1 for decorator "
      "of type: " + decorator->GetType() +
      "] and name [" + decorator->GetName() + "]";
    throw ParseException(error_message);
  }
  auto child_instr = ParseInstruction(children[0], filename);
  if (!child_instr)
  {
    std::string error_message =
      "sup::sequencer::AddChildrenToDecorator(): could not create instruction from TreeData "
      "of type: " + children[0].GetType() +
      "] and name [" + children[0].GetName() + "]";
    throw ParseException(error_message);
  }
  decorator->SetInstruction(child_instr.release());
}

static void AddChildrenToCompound(CompoundInstruction *compound,
                                  const std::vector<TreeData> &children,
                                  const std::string &filename)
{
  for (auto &child : children)
  {
    auto child_instr = ParseInstruction(child, filename);
    if (!child_instr)
    {
      std::string error_message =
        "sup::sequencer::AddChildrenToCompound(): could not create instruction from TreeData "
        "of type: " + child.GetType() +
        "] and name [" + child.GetName() + "]";
      throw ParseException(error_message);
    }
    compound->PushBack(child_instr.release());
  }
}

}  // namespace sequencer

}  // namespace sup
