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

#ifndef SUP_OAC_TREE_DECORATOR_INSTRUCTION_H_
#define SUP_OAC_TREE_DECORATOR_INSTRUCTION_H_

#include <sup/oac-tree/instruction.h>

#include <memory>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Interface class representing a decorated instruction.
 */
class DecoratorInstruction : public Instruction
{
public:
  /**
   * @brief Constructor.
   *
   * @param type The type of instruction.
   */
  DecoratorInstruction(const std::string& type);

  ~DecoratorInstruction() override;

  Category GetCategory() const final;

protected:
  /**
   * @brief Call Instruction::Setup(const Procedure & proc) on child instruction
   * and return result.
   */
  void SetupChild(const Procedure& proc);

  /**
   * @brief Check if this decorator has a child.
   *
   * @return true when it has a child, false otherwise.
   */
  bool HasChild() const;

  /**
   * @brief Call Instruction::Reset() on the child instruction.
   */
  ExecutionStatus GetChildStatus() const;

  /**
   * @brief Call Instruction::ExecuteSingle() on the child instruction.
   */
  void ExecuteChild(UserInterface& ui, Workspace& ws);

  /**
   * @brief Call Instruction::Reset() on the child instruction.
   */
  void ResetChild(UserInterface& ui);

  /**
   * @brief Call Instruction::Halt() on the child instruction.
   */
  void HaltChild();

private:
  void ResetHook(UserInterface& ui) override;
  void HaltImpl() override;
  std::vector<const Instruction*> ChildInstructionsImpl() const override;
  std::vector<const Instruction*> NextInstructionsImpl() const override;
  int ChildrenCountImpl() const override;
  bool InsertInstructionImpl(std::unique_ptr<Instruction>&& child, int index) override;
  std::unique_ptr<Instruction> TakeInstructionImpl(int index) override;
  void SetupImpl(const Procedure &proc) override;
  void AssertChildPresent() const;

  std::unique_ptr<Instruction> m_child;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_DECORATOR_INSTRUCTION_H_
