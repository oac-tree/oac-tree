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

#ifndef SUP_SEQUENCER_CONDITION_H_
#define SUP_SEQUENCER_CONDITION_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Checks a boolean workspace variable.
 */
class Condition : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Returns SUCCESS if the variable is true and FAILURE otherwise. If the variable
   * is not boolean, it will check if the value is different than 0
   */
  virtual ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws);

public:
  /**
   * @brief Constructor.
   */
  Condition();

  /**
   * @brief Destructor.
   */
  virtual ~Condition();

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONDITION_H_
