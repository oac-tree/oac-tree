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

#ifndef SUP_SEQUENCER_RUNNER_H_
#define SUP_SEQUENCER_RUNNER_H_

#include <atomic>

namespace sup
{
namespace sequencer
{

class Procedure;
class UserInterface;

/**
 * @brief Class that aggregates a Procedure and a UserInterface.
 */
class Runner
{
private:
  Procedure* _proc;

  UserInterface* _ui;

  std::atomic_bool halt;

protected:
public:
  /**
   * @brief Constructor.
   * @param ui User interface object.
   */
  Runner(UserInterface* ui);

  /**
   * @brief Destructor.
   */
  ~Runner();

  /**
   * @brief Set the procedure..
   * @param procedure Pointer to procedure.
   */
  void SetProcedure(Procedure* procedure);

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
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_RUNNER_H_
