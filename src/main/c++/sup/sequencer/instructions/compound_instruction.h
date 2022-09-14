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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_COMPOUND_INSTRUCTION_H_
#define SUP_SEQUENCER_COMPOUND_INSTRUCTION_H_

#include <sup/sequencer/instruction.h>

#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief Interface class representing a compound instruction.
 */
class CompoundInstruction : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  void ResetHook() override;
  void HaltImpl() override;
  std::vector<const Instruction *> ChildInstructionsImpl() const override;
  int ChildrenCountImpl() const override;
  bool InsertInstructionImpl(Instruction *child, int index) override;
  Instruction *TakeInstructionImpl(int index) override;
  bool SetupImpl(const Procedure &proc) override;

  /**
   * @brief Container for child instructions.
   */
  std::vector<Instruction *> _children;

protected:
  /**
   * @brief Call Instruction::Setup(const Procedure & proc) on all child instructions
   * and aggregate result.
   */
  bool SetupChildren(const Procedure &proc);

  /**
   * @brief Check if this compound instruction has child instructions.
   *
   * @return true when it has chil instructions, false otherwise.
   */
  bool HasChildren() const;

  /**
   * @brief Call Instruction::Reset() on all child instructions.
   */
  void ResetChildren();

  /**
   * @brief Call Instruction::Halt() on all child instructions.
   */
  void HaltChildren();

public:
  /**
   * @brief Constructor.
   *
   * @param type The type of instruction.
   */
  CompoundInstruction(const std::string &type);

  /**
   * @brief Destructor.
   */
  ~CompoundInstruction() override;

  /**
   * @brief Add child instruction.
   *
   * @param instruction Pointer to instruction.
   * @note CompoundInstruction takes ownership of the added instruction.
   */
  void PushBack(Instruction *instruction);
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_COMPOUND_INSTRUCTION_H_
