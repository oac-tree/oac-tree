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

#include <sup/sequencer/instruction_tree.h>

#include <algorithm>

namespace sup
{
namespace sequencer
{

InstructionTree::InstructionTree(const Instruction* instruction)
  : m_instruction{instruction}
{}

InstructionTree::~InstructionTree()
{
  for (auto tree : m_children)
  {
    delete tree;
  }
}

const Instruction* InstructionTree::GetInstruction() const
{
  return m_instruction;
}

InstructionTree* InstructionTree::AddChildInstruction(const Instruction* instruction)
{
  m_children.push_back(new InstructionTree{instruction});
  return m_children.back();
}

std::vector<const Instruction*> InstructionTree::GetChildInstructions() const
{
  std::vector<const Instruction*> result;
  auto op = [](InstructionTree* tree) {
    return tree->GetInstruction();
  };
  std::transform(m_children.begin(), m_children.end(), std::back_inserter(result), op);
  return result;
}

std::vector<InstructionTree*> InstructionTree::GetChildren() const
{
  return m_children;
}

}  // namespace sequencer

}  // namespace sup
