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

#ifndef SUP_SEQUENCER_CONDITION_H_
#define SUP_SEQUENCER_CONDITION_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Checks a boolean workspace variable.
 *
 * @details Returns SUCCESS if the variable is true and FAILURE otherwise. If the variable
 * is not boolean, it will check if the value is different than 0
 */
class Condition : public Instruction
{
public:
  Condition();

  virtual ~Condition();

  static const std::string Type;

private:
  virtual ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws);
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CONDITION_H_
