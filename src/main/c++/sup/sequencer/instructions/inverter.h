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

#ifndef SUP_SEQUENCER_INVERTER_H_
#define SUP_SEQUENCER_INVERTER_H_

#include "decorator_instruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator that inverts the execution status of its child.
 */
class Inverter : public DecoratorInstruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Inverts the execution status of its child instruction, interchanging
   * SUCCESS and FAILURE.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction.
   */
  ExecutionStatus CalculateStatus() const;

protected:
public:
  /**
   * @brief Constructor.
   */
  Inverter();

  /**
   * @brief Destructor.
   */
  ~Inverter() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INVERTER_H_
