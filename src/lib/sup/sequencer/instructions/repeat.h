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

#ifndef SUP_SEQUENCER_REPEAT_H_
#define SUP_SEQUENCER_REPEAT_H_

#include <sup/sequencer/decorator_instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction decorator that repeats its child a fixed number of times while successful.
 *
 * @details Repeatedly executes the child instruction, until either: the child fails (FAILURE)
 * or maximum number of repetitions is reached (SUCCESS).
 */
class Repeat : public DecoratorInstruction
{
public:
  Repeat();

  ~Repeat() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction
   * and the number of successful executions of the child instruction.
   */
  ExecutionStatus CalculateStatus() const;

  sup::dto::int32 m_max_count;
  sup::dto::int32 m_count;
  bool m_init_ok;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_REPEAT_H_
