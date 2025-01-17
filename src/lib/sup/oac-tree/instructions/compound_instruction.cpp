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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/compound_instruction.h>

#include <sup/oac-tree/constants.h>

namespace sup
{
namespace oac_tree
{
CompoundInstruction::CompoundInstruction(const std::string& type)
  : Instruction(type)
{
  AddAttributeDefinition(Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

CompoundInstruction::~CompoundInstruction() = default;

Instruction::Category CompoundInstruction::GetCategory() const
{
  return kCompound;
}

void CompoundInstruction::SetupChildren(const Procedure& proc)
{
  AssertChildrenPresent();
  for (auto& instruction : m_children)
  {
    instruction->Setup(proc);
  }
}

bool CompoundInstruction::HasChildren() const
{
  return !m_children.empty();
}

void CompoundInstruction::ResetChildren(UserInterface& ui)
{
  for (auto& instruction : m_children)
  {
    instruction->Reset(ui);
  }
}

void CompoundInstruction::HaltChildren()
{
  for (auto& instruction : m_children)
  {
    instruction->Halt();
  }
}

void CompoundInstruction::ResetHook(UserInterface& ui)
{
  ResetChildren(ui);
}

void CompoundInstruction::HaltImpl()
{
  HaltChildren();
}

std::vector<const Instruction*> CompoundInstruction::ChildInstructionsImpl() const
{
  std::vector<const Instruction*> result;
  for (auto& instr : m_children)
  {
    result.push_back(instr.get());
  }
  return result;
}

int CompoundInstruction::ChildrenCountImpl() const
{
  return static_cast<int>(m_children.size());
}

bool CompoundInstruction::InsertInstructionImpl(std::unique_ptr<Instruction>&& child, int index)
{
  if (index < 0 || index > ChildrenCount())
  {
    return false;
  }
  m_children.emplace(std::next(m_children.begin(), index), std::move(child));
  return true;
}

std::unique_ptr<Instruction> CompoundInstruction::TakeInstructionImpl(int index)
{
  if (index < 0 || index >= ChildrenCount())
  {
    return nullptr;
  }
  auto it = std::next(m_children.begin(), index);
  std::unique_ptr<Instruction> retval;
  std::swap(retval, *it);
  m_children.erase(it);
  return std::move(retval);
}

void CompoundInstruction::SetupImpl(const Procedure& proc)
{
  SetupChildren(proc);
}

void CompoundInstruction::AssertChildrenPresent() const
{
  if (!HasChildren())
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "Compound instruction requires at least one child instruction";
    throw InstructionSetupException(error_message);
  }
}


}  // namespace oac_tree

}  // namespace sup
