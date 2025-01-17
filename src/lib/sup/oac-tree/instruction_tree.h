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

#ifndef SUP_OAC_TREE_INSTRUCTION_TREE_H_
#define SUP_OAC_TREE_INSTRUCTION_TREE_H_

#include <vector>

namespace sup
{
namespace oac_tree
{
class Instruction;

/**
 * @brief Tree of instruction pointers.
 */

class InstructionTree
{
public:
  InstructionTree(const Instruction* instruction);

  ~InstructionTree();

  bool IsEmpty() const;

  const Instruction* GetInstruction() const;

  InstructionTree* AddChildInstruction(const Instruction* instruction);

  std::vector<const Instruction*> GetChildInstructions() const;

  std::vector<InstructionTree*> GetChildren() const;

private:
  const Instruction* m_instruction;
  std::vector<InstructionTree*> m_children;
};

/**
 * @brief Create an InstructionTree representing the whole instruction tree starting from the
 * given root instruction.
*/
InstructionTree CreateFullInstructionTree(const Instruction* root);

/**
 * @brief Create an InstructionTree representing the tree of instructions that will be executed next.
*/
InstructionTree CreateNextInstructionTree(const Instruction* root);

/**
 * @brief Retrieve the leaves of an instruction tree.
*/
std::vector<const Instruction*> GetLeaves(const InstructionTree& tree);

/**
 * @brief Flatten an InstructionTree using breadth-first search.
*/
std::vector<const Instruction*> FlattenBFS(const InstructionTree& tree);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_TREE_H_
