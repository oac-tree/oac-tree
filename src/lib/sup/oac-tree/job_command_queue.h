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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_JOB_COMMAND_QUEUE_H_
#define SUP_OAC_TREE_JOB_COMMAND_QUEUE_H_

#include <sup/oac-tree/job_commands.h>

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Threadsafe fifo queue for commands, used in `AsyncRunner`.
*/
class JobCommandQueue
{
public:
  JobCommandQueue();
  ~JobCommandQueue();

  /**
   * @brief Push new command to queue.
   *
   * @param command Command to push.
   */
  void Push(JobCommand command);

  /**
   * @brief Push new command to queue with priority: if the first command in the queue has lower
   * priority than the given command, execute the passed function, push the given command to the
   * front and return true. Otherwise, return false.
   *
   * @param command Command to push.
   * @param func Function to execute if the command was successfully pushed.
   *
   * @return true when the command was successfully pushed to the front of the queue.
   */
  bool PriorityPush(JobCommand command, std::function<void()> func);

  /**
   * @brief Query if command queue is empty.
   *
   * @return true when queue is empty.
   */
  bool IsEmpty() const;

  /**
   * @brief Query the size of the command queue.
   *
   * @return Number of commands currently in the queue.
   */
  std::size_t GetSize() const;

  /**
   * @brief Try to pop a command from the queue.
   *
   * @param command Output parameter that will hold the popped command if successful.
   * @return true when successful. False indicates the queue was empty.
   */
  bool TryPop(JobCommand& command);

  /**
   * @brief Blocks until a command can be retrieved from the queue and returns that command.
   *
   * @return Popped command
   */
  JobCommand WaitForNextCommand();

private:
  std::deque<JobCommand> m_command_queue;
  mutable std::mutex m_mtx;
  std::condition_variable m_cv;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_JOB_COMMAND_QUEUE_H_
