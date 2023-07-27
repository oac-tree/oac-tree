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

#ifndef SUP_SEQUENCER_RUNNER_H_
#define SUP_SEQUENCER_RUNNER_H_

#include <sup/sequencer/breakpoint.h>

#include <atomic>
#include <functional>
#include <vector>

namespace sup
{
namespace sequencer
{

class Instruction;
class Procedure;
class UserInterface;

/**
 * @brief Class that aggregates a Procedure and a UserInterface.
 */
class Runner
{
public:
  using TickCallback = std::function<void(const Procedure&)>;

  /**
   * @brief Constructor.
   * @param ui User interface object.
   */
  Runner(UserInterface& ui);

  ~Runner();

  /**
   * @brief Set the procedure.
   * @param procedure Pointer to procedure.
   */
  void SetProcedure(Procedure* procedure);

  /**
   * @brief Set the callback function to be called after each execution step.
   * @param cb Callback function.
   */
  void SetTickCallback(TickCallback cb = {});

  /**
   * @brief Set a breakpoint at the given instruction.
   * @param instruction Pointer to instruction for breakpoint location.
   *
   * @note This member function should not be called during execution, but only when it is paused,
   * before/after a single execution step or after a breakpoint was triggered.
   */
  void SetBreakpoint(const Instruction* instruction);

  /**
   * @brief Remove a breakpoint at the given instruction (if it exists).
   * @param instruction Pointer to instruction for breakpoint location.
   *
   * @note This member function should not be called during execution, but only when it is paused,
   * before/after a single execution step or after a breakpoint was triggered.
   */
  void RemoveBreakpoint(const Instruction* instruction);

  /**
   * @brief Get a list of breakpoints.
   *
   * @return List of breakpoints.
   *
   * @details This member function returns a copy of the breakpoint list. Manipulation of breakpoint
   * statuses is handled only internally or through dedicated member functions like
   * DisableBreakpoint().
   *
   * @note This member function should not be called during execution, but only when it is paused,
   * before/after a single execution step or after a breakpoint was triggered.
   */
  std::vector<Breakpoint> GetBreakpoints() const;

  /**
   * @brief Execute the procedure
   */
  void ExecuteProcedure();

  /**
   * @brief Execute single procedure instruction.
   */
  void ExecuteSingle();

  /**
   * @brief Get a list of instruction pointers for which breakpoints were triggered.
   *
   * @return List of instructions whose breakpoints were triggered.
   *
   * @note This member function should not be called during execution, but only when it is paused,
   * before/after a single execution step or after a breakpoint was triggered.
   */
  std::vector<const Instruction*> GetCurrentBreakpointInstructions() const;

  /**
   * @brief Halts the procedure execution.
   */
  void Halt();

  /**
   * @brief Pauses the procedure execution.
   *
   * @note Resuming the execution is done by calling ExecuteProcedure() again.
   */
  void Pause();

  /**
   * @brief Query if procedure has finished.
   *
   * @return True if procedure has finished.
   */
  bool IsFinished() const;

  /**
   * @brief Query if procedure is running in seperate threads.
   *
   * @return True if procedure has seperate threads executing.
   */
  bool IsRunning() const;

private:
  bool InstructionPresent(const Instruction* instruction) const;

  Procedure* m_proc;
  UserInterface& m_ui;
  TickCallback m_tick_cb;
  std::vector<Breakpoint> m_breakpoints;
  std::vector<const Instruction*> m_current_breakpoint_instructions;
  std::atomic_bool m_halt;
};

/**
 * @brief Class that can be used as a callback in between ticks. It will perform a fixed timeout
 * when the procedure reports a running status (async operation).
 */
class TimeoutWhenRunning
{
public:
  TimeoutWhenRunning(int ms);
  ~TimeoutWhenRunning();

  void operator()(const Procedure& proc) const;
private:
  int m_timeout_ms;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_RUNNER_H_
