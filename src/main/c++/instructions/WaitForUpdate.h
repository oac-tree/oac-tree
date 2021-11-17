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
 * @file Wait.h
 * @brief Header file for WaitForUpdate instruction.
 * @date 17/11/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the WaitForUpdate instruction.
 */

#ifndef _SEQ_WaitForUpdate_h_
#define _SEQ_WaitForUpdate_h_

#include "Instruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction node that returns SUCCESS after a given timeout.
 */

class WaitForUpdateInstruction : public Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Execution returns SUCCESS if variable was updated before the timeout.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief See sup::sequencer::Instruction.
   */
  bool SetupImpl(const Procedure& proc) override;

  double timeout;

public:
  WaitForUpdateInstruction();
  ~WaitForUpdateInstruction() override;

  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_WaitForUpdate_h_
