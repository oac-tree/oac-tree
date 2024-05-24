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

#ifndef SUP_SEQUENCER_async_runner_H_
#define SUP_SEQUENCER_async_runner_H_

#include <sup/sequencer/job_command_queue.h>
#include <sup/sequencer/job_interface.h>
#include <sup/sequencer/job_state_monitor.h>
#include <sup/sequencer/job_states.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>
#include <sup/sequencer/user_interface.h>

#include <future>

namespace sup
{
namespace sequencer
{

/**
 * @brief Asynchronous wrapper around Runner.
 *
 * The AsyncRunner's responsibility is to asynchronously handle control commands related to a
 * procedure's execution. The main difference with the existing Runner in the Sequencer, which is
 * encapsulated here, is that all interactions with the procedure's execution are asynchronous and
 * that it reports also job progress through the JobStateMonitor.
*/
class AsyncRunner
{
public:
  /**
   * @brief Constructor.
   *
   * @param proc Procedure (should not be setup, that will happen during this constructor).
   * @param ui UserInterface object.
   * @param state_monitor Object that will monitor state and breakpoint changes.
   */
  explicit AsyncRunner(Procedure& proc, UserInterface& ui, JobStateMonitor& state_monitor);

  /**
   * @brief Constructor.
   *
   * @param proc Procedure (should not be setup, that will happen during this constructor).
   * @param job_ui JobInterface object that handles both UserInterface and JobStateMonitor.
   */
  explicit AsyncRunner(Procedure& proc, JobInterface& job_ui);

  ~AsyncRunner();

   /**
   * @brief Set a breakpoint at the given instruction.
   *
   * @param instruction Pointer to instruction for breakpoint location.
   */
  void SetBreakpoint(const Instruction* instruction);

  /**
   * @brief Remove a breakpoint at the given instruction (if it exists).
   *
   * @param instruction Pointer to instruction for breakpoint location.
   */
  void RemoveBreakpoint(const Instruction* instruction);

 /**
   * @brief Run the procedure in a continuous manner if allowed.
   */
  void Start();

  /**
   * @brief Execute a single step of the procedure if allowed.
   */
  void Step();

  /**
   * @brief Pause the procedure's execution.
   */
  void Pause();

  /**
   * @brief Reset the procedure to its initial state.
   */
  void Reset();

  /**
   * @brief Halt the procedure. This prevents continuing it afterwards. The procedure has to be
   * reset to be able to run again.
   */
  void Halt();

private:
  Procedure& m_proc;
  UserInterface& m_ui;
  Runner m_runner;

  JobStateMonitor& m_state_monitor;

  JobCommandQueue m_command_queue;

  /**
   * @brief Action to perform after handling a command.
   */
  enum class Action
  {
    kContinue = 0,
    kStep,
    kRun,
    kExit
  };

  using CommandHandlerFunction = Action (AsyncRunner::*)(JobCommand);
  CommandHandlerFunction m_command_handler;
  /**
   * @brief Track the AsyncRunner's execution loop.
   */
  std::future<void> m_loop_future;

  std::atomic_bool m_keep_alive;

  /**
   * @brief Halts the procedure/runner and exits the execution loop.
   */
  void Terminate();

  void SetState(JobState state);

  void Launch();

  Action HandleCommand(JobCommand command);

  Action HandleInitial(JobCommand command);

  Action HandleRunning(JobCommand command);

  Action HandlePaused(JobCommand command);

  Action HandleFinished(JobCommand command);

  void ExecutionLoop();

  void RunProcedure();

  void ProcessCommandsWhenRunning();

  void StepProcedure();

  /**
   * @brief Check if procedure is finished (success/failure) and switch states accordingly.
   *
   * @return true when procedure was finished.
   */
  bool SwitchStateOnFinished();
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_async_runner_H_
