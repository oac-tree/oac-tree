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

#ifndef SUP_SEQUENCER_INVERTER_H_
#define SUP_SEQUENCER_INVERTER_H_

#include "decorator_instruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator that inverts the execution status of its child, interchanging
 * SUCCESS and FAILURE.
 */
class Inverter : public DecoratorInstruction
{
public:
  Inverter();

  ~Inverter() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction.
   */
  ExecutionStatus CalculateStatus() const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INVERTER_H_
