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

#include <sup/sequencer/instructions/CompoundInstruction.h>
#include <sup/sequencer/instructions/DecoratorInstruction.h>
#include <sup/sequencer/instructions/Include.h>
#include <sup/sequencer/instruction_registry.h>

#include <sup/sequencer/log.h>

namespace sup
{
namespace sequencer
{
static bool AddChildInstructions(Instruction *instruction, const std::vector<TreeData> &children,
                                 const std::string &filename);
static bool AddChildrenToDecorator(DecoratorInstruction *decorator,
                                   const std::vector<TreeData> &children,
                                   const std::string &filename);
static bool AddChildrenToCompound(CompoundInstruction *compound,
                                  const std::vector<TreeData> &children,
                                  const std::string &filename);

std::unique_ptr<Instruction> ParseInstruction(const TreeData &data, const std::string &filename)
{
  auto instr_type = data.GetType();
  auto instr = GlobalInstructionRegistry().Create(instr_type);
  if (!instr)
  {
    log::Warning("sup::sequencer::ParseInstruction() - couldn't parse instruction with type: '%s'",
                instr_type.c_str());
    return {};
  }
  for (auto &attr : data.Attributes())
  {
    instr->AddAttribute(attr.first, attr.second);
  }
  log::Debug(
      "sup::sequencer::ParseInstruction() - "
      "parsing child instructions for instruction of type: '%s'",
      instr_type.c_str());
  bool status = AddChildInstructions(instr.get(), data.Children(), filename);
  if (!status)
  {
    log::Warning(
        "sup::sequencer::ParseInstruction() - instruction with type: '%s' parsing child "
        "instructions failed..",
        instr->GetType().c_str());
    return {};
  }
  return instr;
}

static bool AddChildInstructions(Instruction *instruction, const std::vector<TreeData> &children,
                                 const std::string &filename)
{
  auto instr_name = instruction->GetName();
  auto instr_type = instruction->GetType();

  // Only set source directory for Include instruction:
  auto include = dynamic_cast<Include *>(instruction);
  if (include)
  {
    include->SetFilename(filename);
    return true;
  }

  auto decorator = dynamic_cast<DecoratorInstruction *>(instruction);
  if (decorator)
  {
    log::Debug("AddChildInstructions() - (%s:%s)", instr_type.c_str(), instr_name.c_str());
    return AddChildrenToDecorator(decorator, children, filename);
  }

  auto compound = dynamic_cast<CompoundInstruction *>(instruction);
  if (compound)
  {
    log::Debug("AddChildInstructions() - (%s:%s)", instr_type.c_str(), instr_name.c_str());
    if (AddChildrenToCompound(compound, children, filename))
    {
      return true;
    }
    log::Warning("AddChildInstructions() - could not parse child instruction of (%s:%s)",
                instr_type.c_str(), instr_name.c_str());
    return false;
  }
  if (children.empty())
  {
    // Leaf instructions don't have children, so ok.
    return true;
  }

  return false;
}

static bool AddChildrenToDecorator(DecoratorInstruction *decorator,
                                   const std::vector<TreeData> &children,
                                   const std::string &filename)
{
  if (children.size() == 1u)
  {
    auto child_instr = ParseInstruction(children[0], filename);
    if (child_instr)
    {
      auto child_type = child_instr->GetType();
      log::Debug("AddChildrenToDecorator() - calling Decorator->SetInstruction(%s)",
                child_type.c_str());
      decorator->SetInstruction(child_instr.release());
      return true;
    }
  }
  return false;
}

static bool AddChildrenToCompound(CompoundInstruction *compound,
                                  const std::vector<TreeData> &children,
                                  const std::string &filename)
{
  if (children.size() > 0)
  {
    bool result = true;
    for (auto &child : children)
    {
      auto child_instr = ParseInstruction(child, filename);
      if (child_instr)
      {
        auto child_type = child_instr->GetType();
        log::Debug("AddChildrenToCompound() - calling Compound->PushBack(%s)", child_type.c_str());
        compound->PushBack(child_instr.release());
        continue;
      }
      result = false;
    }
    return result;
  }
  return false;
}

}  // namespace sequencer

}  // namespace sup
