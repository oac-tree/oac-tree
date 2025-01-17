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

#include <sup/oac-tree/instruction_tree.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>

#include <algorithm>
#include <deque>
#include <functional>

namespace sup
{
namespace oac_tree
{
using InstructionChildSelector = std::function<std::vector<const Instruction*>(const Instruction*)>;

InstructionTree CreateInstructionTree(const Instruction* root, InstructionChildSelector selector);

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

bool InstructionTree::IsEmpty() const
{
  return m_instruction == nullptr;
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

InstructionTree CreateFullInstructionTree(const Instruction* root)
{
  auto selector = [](const Instruction* instruction) {
    return instruction->ChildInstructions();
  };
  return CreateInstructionTree(root, selector);
}

InstructionTree CreateNextInstructionTree(const Instruction* root)
{
  auto selector = [](const Instruction* instruction) {
    return instruction->NextInstructions();
  };
  return CreateInstructionTree(root, selector);
}

InstructionTree CreateInstructionTree(const Instruction* root, InstructionChildSelector selector)
{
  if (root == nullptr)
  {
    std::string message = "CreateInstructionTree(): called with nullptr to instruction";
    throw InvalidOperationException(message);
  }
  std::deque<InstructionTree*> stack;
  InstructionTree result{root};
  stack.push_back(std::addressof(result));
  while (!stack.empty())
  {
    auto tree = stack.back();
    stack.pop_back();
    for (auto instr : selector(tree->GetInstruction()))
    {
      if (instr == nullptr)
      {
        std::string message = "CreateInstructionTree(): called with nullptr child";
        throw InvalidOperationException(message);
      }
      stack.push_back(tree->AddChildInstruction(instr));
    }
  }
  return result;
}

std::vector<const Instruction*> GetLeaves(const InstructionTree& tree)
{
  std::vector<const Instruction*> result;
  std::deque<const InstructionTree*> stack;
  std::deque<const InstructionTree*> temp_stack;
  stack.push_back(&tree);
  while(!stack.empty())
  {
    while (!stack.empty())
    {
      auto node = stack.front();
      stack.pop_front();
      auto child_nodes = node->GetChildren();
      for (auto child_node : child_nodes)
      {
        temp_stack.push_back(child_node);
      }
      if (child_nodes.empty())
      {
        result.push_back(node->GetInstruction());
      }
    }
    std::swap(stack, temp_stack);
  }
  return result;
}

std::vector<const Instruction*> FlattenBFS(const InstructionTree& tree)
{
  std::vector<const Instruction*> result;
  std::deque<const InstructionTree*> stack;
  std::deque<const InstructionTree*> temp_stack;
  stack.push_back(&tree);
  while(!stack.empty())
  {
    while (!stack.empty())
    {
      auto node = stack.front();
      stack.pop_front();
      result.push_back(node->GetInstruction());
      for (auto child_node : node->GetChildren())
      {
        temp_stack.push_back(child_node);
      }
    }
    std::swap(stack, temp_stack);
  }
  return result;
}

}  // namespace oac_tree

}  // namespace sup
