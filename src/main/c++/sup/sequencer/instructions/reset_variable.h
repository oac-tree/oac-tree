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

#ifndef SUP_SEQUENCER_RESET_VARIABLE_H_
#define SUP_SEQUENCER_RESET_VARIABLE_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction to reset a variable to its initial state.
 */
class ResetVariable : public Instruction
{
public:
  ResetVariable();

  ~ResetVariable() override;

  static const std::string Type;

private:
  void SetupImpl(const Procedure &proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_RESET_VARIABLE_H_
