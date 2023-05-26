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

#ifndef SUP_SEQUENCER_DECORATOR_INSTRUCTION_H_
#define SUP_SEQUENCER_DECORATOR_INSTRUCTION_H_

#include <sup/sequencer/instruction.h>

#include <memory>

namespace sup
{
namespace sequencer
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

  /**
   * @brief Set the decorated child instruction.
   *
   * @param instruction Pointer to decorated instruction.
   * @note DecoratorInstruction takes ownership of the decorated instruction.
   */
  void SetInstruction(Instruction* instruction);

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
  void ResetChild();

  /**
   * @brief Call Instruction::Halt() on the child instruction.
   */
  void HaltChild();

private:
  void ResetHook() override;
  void HaltImpl() override;
  std::vector<const Instruction *> ChildInstructionsImpl() const override;
  int ChildrenCountImpl() const override;
  bool InsertInstructionImpl(Instruction *child, int index) override;
  Instruction *TakeInstructionImpl(int index) override;
  void SetupImpl(const Procedure &proc) override;

  std::unique_ptr<Instruction> m_child;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_DECORATOR_INSTRUCTION_H_
