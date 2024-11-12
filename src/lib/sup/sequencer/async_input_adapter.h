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

#ifndef SUP_SEQUENCER_ASYNC_INPUT_ADAPTER_H_
#define SUP_SEQUENCER_ASYNC_INPUT_ADAPTER_H_

#include <sup/sequencer/i_user_input_future.h>
#include <sup/sequencer/user_input_request.h>

#include <sup/dto/basic_scalar_types.h>

#include <functional>
#include <future>
#include <list>
#include <map>
#include <mutex>

namespace sup
{
namespace sequencer
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
  // query readiness of a user input request
  bool UserInputRequestReady(const Future& token) const;

  // throws if the input was not ready or no request is active
  UserInputReply GetReply(const Future& token);

  // cancel a user input request
  void CancelInputRequest(const Future& token);

  sup::dto::uint64 GetNewRequestId();
  void CleanUpUnused();
  InputFunction m_input_func;
  InterruptFunction m_interrupt_func;
  std::map<sup::dto::uint64, std::future<UserInputReply>> m_requests;
  sup::dto::uint64 m_last_request_id;
  std::list<sup::dto::uint64> m_cancelled;
  mutable std::mutex m_mtx;
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

  UserInputReply GetValue() override;

private:
  AsyncInputAdapter& m_input_handler;
  sup::dto::uint64 m_id;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ASYNC_INPUT_ADAPTER_H_
