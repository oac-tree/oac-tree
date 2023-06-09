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
   * @brief Set a breakpoint at the given instruction.
   * @param instruction Pointer to instruction for breakpoint location.
   */
  void SetBreakpoint(const Instruction* instruction);

  /**
   * @brief Execute the procedure
   */
  void ExecuteProcedure();

  /**
   * @brief Execute single procedure instruction.
   */
  void ExecuteSingle();

  /**
   * @brief Halts the procedure execution.
   */
  void Halt();

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
  Procedure* m_proc;
  UserInterface& m_ui;
  std::vector<Breakpoint> m_breakpoints;
  std::atomic_bool m_halt;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_RUNNER_H_
