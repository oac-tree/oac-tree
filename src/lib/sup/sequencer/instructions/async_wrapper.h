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

#ifndef SUP_SEQUENCER_ASYNC_WRAPPER_H_
#define SUP_SEQUENCER_ASYNC_WRAPPER_H_

#include <sup/sequencer/instruction.h>

#include <future>
#include <memory>

namespace sup
{
namespace sequencer
{
/**
 * @brief Wrapper to execute an Instruction asynchronously.
 */
class AsyncWrapper
{
public:
  /**
   * @brief Constructor.
   * @param instruction The wrapped Instruction.
   */
  AsyncWrapper(Instruction* instruction);

  /**
   * @brief Move constructor.
   *
   * @note The move constructor is needed for storage in STL containers.
   */
  AsyncWrapper(AsyncWrapper&& other);

  ~AsyncWrapper();

  /**
   * @brief Execution method.
   *
   * @details Launches the ExecuteSingle method on the wrapped instruction
   * asynchronously if it is not already running.
   */
  void Tick(UserInterface& ui, Workspace& ws);

  /**
   * @brief Try to fetch execution status of child instruction
   */
  void FetchStatus();

  /**
   * @brief Get execution status
   */
  ExecutionStatus GetStatus() const;

private:
  /**
   * @brief Wrapped instruction (not owned!)
   */
  Instruction* m_instruction;

  /**
   * @brief Execution status of the wrapper
   */
  ExecutionStatus m_status;

  /**
   * @brief Track the wrapped instruction
   */
  std::future<void> m_child_result;

  /**
   * @brief Execution status of the wrapper
   */
  void LaunchChild(UserInterface& ui, Workspace& ws);

  /**
   * @brief Check if child is still running
   */
  bool ChildIsRunning() const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ASYNC_WRAPPER_H_
