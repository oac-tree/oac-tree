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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_PARALLEL_SEQUENCE_H_
#define SUP_SEQUENCER_PARALLEL_SEQUENCE_H_

#include "async_wrapper.h"
#include <sup/sequencer/compound_instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Compound instruction representing a parallel sequence.
 *
 * @note ParallelSequence runs its child instructions concurrently and
 * returns SUCCESS when enough child instructions finish successfully (greater
 * than or equal to the success threshold). It fails when the failure threshold
 * is reached or the success threshold can no longer be reached. It returns
 * RUNNING otherwise.
 */
class ParallelSequence : public CompoundInstruction
{
public:
  ParallelSequence();

  ~ParallelSequence() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook(UserInterface& ui) override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  /**
   * @brief Calculate this instruction's status from the status of its child instructions.
   */
  ExecutionStatus CalculateCompoundStatus() const;

  /**
   * @brief Initialize synchronous wrappers that encapsulate the asynchronous execution
   * of each child instruction.
   */
  void InitWrappers();

  bool InitThresholds(UserInterface& ui, Workspace& ws);

  std::vector<AsyncWrapper> m_wrappers;

  sup::dto::uint32 m_success_th;
  sup::dto::uint32 m_failure_th;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PARALLEL_SEQUENCE_H_
