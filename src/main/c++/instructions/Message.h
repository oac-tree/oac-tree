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
 * Copyright (c) : 2010-2021 ITER Organization,
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
 * @file Copy.h
 * @brief Header file for Message instruction.
 * @date 01/12/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the Message instruction.
 */

#ifndef _SEQ_Message_h_
#define _SEQ_Message_h_

#include "Instruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction forwarding a text message to the UserInterface implementation.
 */
class Message : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  bool SetupImpl(const Procedure& proc) override;
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

protected:
public:
  /**
   * @brief Constructor.
   */
  Message();

  /**
   * @brief Destructor.
   */
  ~Message() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_Message_h_
