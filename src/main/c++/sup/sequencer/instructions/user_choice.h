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

#ifndef SUP_SEQUENCER_USER_CHOICE_H_
#define SUP_SEQUENCER_USER_CHOICE_H_

#include "compound_instruction.h"

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
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Ask the user to select which of its child instructions should execute.
   * The returned ExecutionStatus is the one from the selected child or FAILURE if no
   * child was selected.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief Construct a list of choices from the available child instructions.
   */
  std::vector<std::string> GetChoices() const;

public:
  /**
   * @brief Constructor.
   */
  UserChoice();

  /**
   * @brief Destructor.
   */
  ~UserChoice() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_CHOICE_H_
