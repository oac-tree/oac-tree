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

#ifndef SUP_SEQUENCER_INSTRUCTION_TREE_H_
#define SUP_SEQUENCER_INSTRUCTION_TREE_H_

#include <functional>
#include <vector>

namespace sup
{
namespace sequencer
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

  const Instruction* GetInstruction() const;

  InstructionTree* AddChildInstruction(const Instruction* instruction);

  std::vector<const Instruction*> GetChildInstructions() const;

  std::vector<InstructionTree*> GetChildren() const;

private:
  const Instruction* m_instruction;
  std::vector<InstructionTree*> m_children;
};

using InstructionChildSelector = std::function<std::vector<const Instruction*>(const Instruction*)>;

InstructionTree CreateInstructionTree(const Instruction* root, InstructionChildSelector selector);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_TREE_H_
