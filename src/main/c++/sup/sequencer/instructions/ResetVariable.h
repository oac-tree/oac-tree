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

#ifndef _SEQ_ResetVariable_h_
#define _SEQ_ResetVariable_h_

#include <sup/sequencer/Instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction to reset a variable to its initial state.
 */
class ResetVariable : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

protected:
public:
  /**
   * @brief Constructor.
   */
  ResetVariable();

  /**
   * @brief Destructor.
   */
  ~ResetVariable() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_ResetVariable_h_
