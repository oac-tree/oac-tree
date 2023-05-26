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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/compound_instruction.h>

namespace sup
{
namespace sequencer
{
CompoundInstruction::CompoundInstruction(const std::string& type) : Instruction(type) {}

CompoundInstruction::~CompoundInstruction()
{
  for (auto instruction : _children)
  {
    delete instruction;
  }
}

void CompoundInstruction::PushBack(Instruction* instruction)
{
  _children.push_back(instruction);
}

void CompoundInstruction::SetupChildren(const Procedure& proc)
{
  for (auto instruction : _children)
  {
    instruction->Setup(proc);
  }
}

bool CompoundInstruction::HasChildren() const
{
  return !_children.empty();
}

void CompoundInstruction::ResetChildren()
{
  for (auto instruction : _children)
  {
    instruction->Reset();
  }
}

void CompoundInstruction::HaltChildren()
{
  for (auto instruction : _children)
  {
    instruction->Halt();
  }
}

void CompoundInstruction::ResetHook()
{
  ResetChildren();
}

void CompoundInstruction::HaltImpl()
{
  HaltChildren();
}

std::vector<const Instruction*> CompoundInstruction::ChildInstructionsImpl() const
{
  std::vector<const Instruction*> result;
  for (auto instr : _children)
  {
    result.push_back(instr);
  }
  return result;
}

int CompoundInstruction::ChildrenCountImpl() const
{
  return static_cast<int>(_children.size());
}

bool CompoundInstruction::InsertInstructionImpl(Instruction* child, int index)
{
  if (index < 0 || index > ChildrenCount())
    return false;
  _children.insert(std::next(_children.begin(), index), child);
  return true;
}

Instruction* CompoundInstruction::TakeInstructionImpl(int index)
{
  if (index < 0 || index >= ChildrenCount())
    return nullptr;

  auto it = std::next(_children.begin(), index);
  auto retval = *it;
  _children.erase(it);
  return retval;
}

void CompoundInstruction::SetupImpl(const Procedure& proc)
{
  SetupChildren(proc);
}

}  // namespace sequencer

}  // namespace sup
