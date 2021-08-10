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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/**
 * @file ForceSuccess.h
 * @brief Header file for ForceSuccess decorator class.
 * @date 16/12/2020
 * @author Giuseppe Ferro (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the ForceSuccess decorator class.
 */

#ifndef _SEQ_ForceSuccess_h_
#define _SEQ_ForceSuccess_h_

#include "DecoratorInstruction.h"

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

#endif /* _SEQ_ForceSuccess_h_ */
