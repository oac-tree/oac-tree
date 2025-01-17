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

#ifndef SUP_OAC_TREE_COMPOUND_INSTRUCTION_H_
#define SUP_OAC_TREE_COMPOUND_INSTRUCTION_H_

#include <sup/oac-tree/instruction.h>

#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Interface class representing a compound instruction.
 */
class CompoundInstruction : public Instruction
{
public:
  /**
   * @brief Constructor.
   *
   * @param type The type of instruction.
   */
  CompoundInstruction(const std::string& type);

  ~CompoundInstruction() override;

  Category GetCategory() const final;

protected:
  /**
   * @brief Call Instruction::Setup(const Procedure & proc) on all child instructions
   * and aggregate result.
   */
  void SetupChildren(const Procedure& proc);

  /**
   * @brief Check if this compound instruction has child instructions.
   *
   * @return true when it has child instructions, false otherwise.
   */
  bool HasChildren() const;

  /**
   * @brief Call Instruction::Reset() on all child instructions.
   */
  void ResetChildren(UserInterface& ui);

  /**
   * @brief Call Instruction::Halt() on all child instructions.
   */
  void HaltChildren();

private:
  void ResetHook(UserInterface& ui) override;
  void HaltImpl() override;
  std::vector<const Instruction *> ChildInstructionsImpl() const override;
  int ChildrenCountImpl() const override;
  bool InsertInstructionImpl(std::unique_ptr<Instruction>&& child, int index) override;
  std::unique_ptr<Instruction> TakeInstructionImpl(int index) override;
  void SetupImpl(const Procedure &proc) override;
  void AssertChildrenPresent() const;

  std::vector<std::unique_ptr<Instruction>> m_children;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_COMPOUND_INSTRUCTION_H_
