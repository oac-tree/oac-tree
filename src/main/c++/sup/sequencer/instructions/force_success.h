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

#ifndef SUP_SEQUENCER_FORCE_SUCCESS_H_
#define SUP_SEQUENCER_FORCE_SUCCESS_H_

#include "decorator_instruction.h"

namespace sup
{
namespace sequencer
{
class ForceSuccess : public DecoratorInstruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Ignore a child's failure and return SUCCESS if the child has
   * finished (SUCCESS/FAILURE).
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

public:
  /**
   * @brief Constructor.
   */
  ForceSuccess();

  /**
   * @brief Destructor.
   */
  ~ForceSuccess() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif /* SUP_SEQUENCER_FORCE_SUCCESS_H_ */
