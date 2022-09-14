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

#ifndef SUP_SEQUENCER_REPEAT_H_
#define SUP_SEQUENCER_REPEAT_H_

#include "decorator_instruction.h"

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction decorator that repeats its child a fixed number of times while successful.
 */
class Repeat : public DecoratorInstruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  void InitHook() override;

  /**
   * @brief See sup::sequencer::Instruction.
   *
   * @details Repeatedly execute the child instruction, until either: the child fails (FAILURE)
   * or maximum number of repetitions is reached (SUCCESS).
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  /**
   * @brief See sup::sequencer::Instruction.
   */
  bool SetupImpl(const Procedure& proc) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction
   * and the number of successful executions of the child instruction.
   */
  ExecutionStatus CalculateStatus() const;

  int _max_count, _count;

  bool _init_ok;

protected:
public:
  /**
   * @brief Constructor.
   */
  Repeat();

  /**
   * @brief Destructor.
   */
  ~Repeat() override;

  /**
   * @brief The instruction's typename.
   */
  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_REPEAT_H_
