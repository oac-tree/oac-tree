/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
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

#ifndef SUP_OAC_TREE_ASYNC_WRAPPER_H_
#define SUP_OAC_TREE_ASYNC_WRAPPER_H_

#include <sup/oac-tree/instruction.h>

#include <future>
#include <memory>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Wrapper to execute an Instruction asynchronously.
 *
 * @details This wrapper is used in ParallelSequence to decouple the asynchronous execution of an
 * instruction from querying its status. If no intermediate ticks were executed after the last call
 * to UpdateStatus and GetStatus, the returned status can give some guarantees on the state of the
 * execution thread of the wrapped instruction:
 * - NOT_STARTED: there is no thread and no shared state.
 * - NOT_FINISHED, SUCCESS, FAILURE: there is a thread that has finished execution. We can safely
 *   destroy the shared state owned by this wrapper or call non threadsafe methods on the wrapped
 *   instruction.
 * - RUNNING: there is a thread that has possibly finished execution. We cannot safely call methods
 *   on the wrapped instruction if these are not threadsafe.
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
   * @brief Get execution status
   */
  ExecutionStatus GetStatus() const;

  /**
   * @brief Get wrapped instruction
   */
  const Instruction* GetInstruction() const;

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
   * @brief Check if AsyncWrapper's thread is (still) running.
   */
  bool WaitingForThread() const;

  /**
   * @brief Update status with status of child instruction and return true when this changes the
   * status of this AsyncWrapper.
   *
   * @note This method should only be called when there is no thread still executing.
   */
  bool UpdateStatus();

  /**
   * @brief Execution status of the wrapper
   */
  void LaunchChild(UserInterface& ui, Workspace& ws);
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ASYNC_WRAPPER_H_
