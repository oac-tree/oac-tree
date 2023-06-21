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

#ifndef SUP_SEQUENCER_USER_CHOICE_H_
#define SUP_SEQUENCER_USER_CHOICE_H_

#include <sup/sequencer/compound_instruction.h>

#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief Compound instruction representing a choice of instructions.
 *
 * @note The selection of the instruction to execute is done by delegating this to
 * the UserInterface.
 */
class UserChoice : public CompoundInstruction
{
public:
  UserChoice();

  ~UserChoice() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook() override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  /**
   * @brief Construct a list of choices from the available child instructions.
   */
  std::vector<std::pair<std::string, int>> GetChoices() const;

  int m_choice;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_CHOICE_H_
