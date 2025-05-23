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

#include <sup/oac-tree/decorator_instruction.h>

#include <sup/oac-tree/constants.h>

namespace sup
{
namespace oac_tree
{
DecoratorInstruction::DecoratorInstruction(const std::string &type)
  : Instruction(type)
  , m_child{}
{
  AddAttributeDefinition(Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

DecoratorInstruction::~DecoratorInstruction() = default;

Instruction::Category DecoratorInstruction::GetCategory() const
{
  return kDecorator;
}

Instruction* DecoratorInstruction::GetChild()
{
  return m_child.get();
}

void DecoratorInstruction::SetupChild(const Procedure &proc)
{
  AssertChildPresent();
  m_child->Setup(proc);
}

bool DecoratorInstruction::HasChild() const
{
  return static_cast<bool>(m_child);
}

ExecutionStatus DecoratorInstruction::GetChildStatus() const
{
  AssertChildPresent();
  return m_child->GetStatus();
}

void DecoratorInstruction::ExecuteChild(UserInterface& ui, Workspace& ws)
{
  AssertChildPresent();
  m_child->ExecuteSingle(ui, ws);
}

void DecoratorInstruction::ResetChild(UserInterface& ui)
{
  if (m_child)
  {
    m_child->Reset(ui);
  }
}

void DecoratorInstruction::HaltChild()
{
  if (m_child)
  {
    m_child->Halt();
  }
}

void DecoratorInstruction::ResetHook(UserInterface& ui)
{
  ResetChild(ui);
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

bool DecoratorInstruction::InsertInstructionImpl(std::unique_ptr<Instruction>&& child, int index)
{
  if (index != 0)
  {
    return false;
  }
  std::swap(m_child, child);
  return true;
}

std::unique_ptr<Instruction> DecoratorInstruction::TakeInstructionImpl(int index)
{
  return m_child && index == 0 ? std::move(m_child) : nullptr;
}

void DecoratorInstruction::SetupImpl(const Procedure &proc)
{
  SetupChild(proc);
}

void DecoratorInstruction::AssertChildPresent() const
{
  if (!m_child)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "Decorator instruction requires a child instruction";
    throw InstructionSetupException(error_message);
  }
}

}  // namespace oac_tree

}  // namespace sup
