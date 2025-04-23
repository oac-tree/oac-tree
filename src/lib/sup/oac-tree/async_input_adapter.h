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

#ifndef SUP_OAC_TREE_ASYNC_INPUT_ADAPTER_H_
#define SUP_OAC_TREE_ASYNC_INPUT_ADAPTER_H_

#include <sup/oac-tree/i_user_input_future.h>
#include <sup/oac-tree/user_input_request.h>

#include <sup/dto/basic_scalar_types.h>

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <map>
#include <mutex>
#include <utility>

namespace sup
{
namespace oac_tree
{

class AsyncInputAdapter
{
public:
  using InputFunction = std::function<UserInputReply(const UserInputRequest&, sup::dto::uint64)>;
  using InterruptFunction = std::function<void(sup::dto::uint64)>;
  class Future;
  explicit AsyncInputAdapter(InputFunction input_func, InterruptFunction interrupt_func);
  ~AsyncInputAdapter();

  // return a future that can be used to check if the result is ready and get it
  std::unique_ptr<IUserInputFuture> AddUserInputRequest(const UserInputRequest& request);

private:
  using RequestEntry = std::pair<sup::dto::uint64, UserInputRequest>;

  // Handle all requests in a single thread.
  void HandleRequestQueue();

  // Create a new input request id
  sup::dto::uint64 GetNewRequestId();

  // query readiness of a user input request
  bool UserInputRequestReady(const Future& token) const;

  // wait for readiness with a timeout
  bool WaitForRequestReady(const Future& token, double seconds);

  // throws if the input was not ready or no request is active
  UserInputReply GetReply(const Future& token);

  // cancel a user input request
  void CancelInputRequest(const Future& token);

  InputFunction m_input_func;
  InterruptFunction m_interrupt_func;
  std::deque<RequestEntry> m_request_queue;
  std::map<sup::dto::uint64, UserInputReply> m_replies;
  sup::dto::uint64 m_current_id;
  sup::dto::uint64 m_last_request_id;
  std::future<void> m_handler_future;
  mutable std::mutex m_mtx;
  std::condition_variable m_cv;
  std::condition_variable m_reply_cv;
  bool m_halt;
};

class AsyncInputAdapter::Future : public IUserInputFuture
{
public:
  Future(AsyncInputAdapter& input_handler, sup::dto::uint64 id);
  ~Future();

  // No move/copy ctor/assignment
  Future(const Future&) = delete;
  Future& operator=(const Future&) = delete;
  Future(Future&& other) = delete;
  Future& operator=(Future&& other) = delete;

  sup::dto::uint64 GetId() const override;

  bool IsValid() const override;
  bool IsReady() const override;
  bool WaitFor(double seconds) const override;

  UserInputReply GetValue() override;

private:
  AsyncInputAdapter& m_input_handler;
  sup::dto::uint64 m_id;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ASYNC_INPUT_ADAPTER_H_
