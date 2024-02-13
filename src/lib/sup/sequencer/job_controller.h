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

#ifndef SUP_SEQUENCER_JOB_CONTROLLER_H_
#define SUP_SEQUENCER_JOB_CONTROLLER_H_

#include <sup/sequencer/job_command_queue.h>
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
 * @brief Asynchronous wrapper around sup::sequencer::Runner.
 *
 * The JobController's responsibility is to asynchronously handle control commands related to a
 * procedure's execution. The main difference with the existing Runner in the Sequencer, which is
 * encapsulated here, is that starting or resuming execution will spawn a thread to handle the
 * procedure's execution.
*/
class JobController
{
public:
  /**
   * @brief Constructor.
   *
   * @param proc Procedure that needs to be correctly setup.
   * @param ui UserInterface object.
   * @param cb Callback function object for state changes.
   */
  explicit JobController(sup::sequencer::Procedure& proc, sup::sequencer::UserInterface& ui,
                         JobStateMonitor& state_monitor);

  ~JobController();

  /**
   * @brief Run the procedure in a continuous manner if allowed.
   */
  void Start();

  /**
   * @brief Execute a single step of the procedure if allowed.
   */
  void Step();

  /**
   * @brief Interrupt the procedure's execution.
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
  sup::sequencer::Procedure& m_proc;
  sup::sequencer::UserInterface& m_ui;
  sup::sequencer::Runner m_runner;

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

  using CommandHandlerFunction = Action (JobController::*)(JobCommand);
  CommandHandlerFunction m_command_handler;
  /**
   * @brief Track the JobController's execution loop.
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

/**
 * @brief Class that is used as an empty callback during stepwise execution. This prevents that
 * there could be two consecutive command handling loops. See `JobController::ExecutionLoop` and
 * the callback used during continuous execution.
 */
class EmptyTickCallback
{
public:
  EmptyTickCallback();
  ~EmptyTickCallback();

  void operator()(const sup::sequencer::Procedure& proc) const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_CONTROLLER_H_
