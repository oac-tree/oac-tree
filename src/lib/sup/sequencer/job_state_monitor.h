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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_JOB_STATE_MONITOR_H_
#define SUP_SEQUENCER_JOB_STATE_MONITOR_H_

#include <sup/sequencer/job_states.h>

namespace sup
{
namespace sequencer
{
class Instruction;
class Procedure;

/**
 * @brief Pure interface for objects that can receive JobState updates from AsyncRunner.
 *
 * @note The member functions are all declared noexcept as implementations should not throw.
 */
class JobStateMonitor
{
public:
  virtual ~JobStateMonitor();

  /**
   * @brief This member function will be called every time the AsyncRunner changes state.
   *
   * @param state New state of the AsyncRunner.
   */
  virtual void OnStateChange(JobState state) noexcept = 0;

  /**
   * @brief This member function will be called every time a breakpoint is set or removed.
   *
   * @param instruction Pointer to the Instruction for which the breakpoint status changed.
   * @param breakpoint_set Boolean indicating if breakpoint was set (true) or removed (false).
   */
  virtual void OnBreakpointChange(const Instruction* instruction, bool breakpoint_set) noexcept = 0;

  /**
   * @brief This member function will be called after each tick during execution of
   * the AsyncRunner.
   *
   * @note This member function can be overridden to implement queries on the procedure that could
   * possibly interfere with execution. Since it is called between ticks, it is safe to call for
   * example `GetNextLeaves(const Procedure& proc)`.
   *
   * @param proc Procedure being executed.
   */
  virtual void OnProcedureTick(const Procedure& proc) = 0;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_STATE_MONITOR_H_
