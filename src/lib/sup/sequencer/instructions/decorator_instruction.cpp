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

#include <sup/sequencer/decorator_instruction.h>

namespace sup
{
namespace sequencer
{
DecoratorInstruction::DecoratorInstruction(const std::string &type)
  : Instruction(type)
  , m_child{}
{}

DecoratorInstruction::~DecoratorInstruction() = default;

Instruction::Category DecoratorInstruction::GetCategory() const
{
  return kDecorator;
}

void DecoratorInstruction::SetInstruction(Instruction *instruction)
{
  m_child.reset(instruction);
}

void DecoratorInstruction::SetupChild(const Procedure &proc)
{
  if (m_child)
  {
    m_child->Setup(proc);
  }
}

bool DecoratorInstruction::HasChild() const
{
  return static_cast<bool>(m_child);
}

ExecutionStatus DecoratorInstruction::GetChildStatus() const
{
  if (m_child)
  {
    return m_child->GetStatus();
  }
  return ExecutionStatus::NOT_STARTED;
}

void DecoratorInstruction::ExecuteChild(UserInterface& ui, Workspace& ws)
{
  if (m_child)
  {
    m_child->ExecuteSingle(ui, ws);
  }
}

void DecoratorInstruction::ResetChild()
{
  if (m_child)
  {
    m_child->Reset();
  }
}

void DecoratorInstruction::HaltChild()
{
  if (m_child)
  {
    m_child->Halt();
  }
}

void DecoratorInstruction::ResetHook()
{
  ResetChild();
}

void DecoratorInstruction::HaltImpl()
{
  HaltChild();
}

std::vector<const Instruction*> DecoratorInstruction::ChildInstructionsImpl() const
{
  std::vector<const Instruction*> result;
  if (m_child)
  {
    result.push_back(m_child.get());
  }
  return result;
}

std::vector<const Instruction*> DecoratorInstruction::NextInstructionsImpl() const
{
  // If the decorator instruction is not finished yet, the child, if present, is next...
  std::vector<const Instruction*> result;
  if (HasChild())
  {
    result.push_back(m_child.get());
  }
  return result;
}

int DecoratorInstruction::ChildrenCountImpl() const
{
  return m_child ? 1 : 0;
}

bool DecoratorInstruction::InsertInstructionImpl(Instruction *child, int index)
{
  if (index != 0)
    return false;
  m_child.reset(child);
  return true;
}

Instruction* DecoratorInstruction::TakeInstructionImpl(int index)
{
  return m_child && index == 0 ? m_child.release() : nullptr;
}

void DecoratorInstruction::SetupImpl(const Procedure &proc)
{
  SetupChild(proc);
}

}  // namespace sequencer

}  // namespace sup
