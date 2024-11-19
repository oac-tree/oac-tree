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

#include <sup/sequencer/async_input_adapter.h>

#include <sup/sequencer/exceptions.h>

#include <algorithm>
#include <chrono>
#include <utility>

namespace sup
{
namespace sequencer
{

AsyncInputAdapter::AsyncInputAdapter(InputFunction input_func, InterruptFunction interrupt_func)
  : m_input_func{std::move(input_func)}
  , m_interrupt_func{std::move(interrupt_func)}
  , m_request_queue{}
  , m_replies{}
  , m_current_id{0}
  , m_last_request_id{0}
  , m_handler_future{}
  , m_mtx{}
  , m_cv{}
  , m_halt{false}
{
  m_handler_future = std::async(std::launch::async, &AsyncInputAdapter::HandleRequestQueue, this);
}

AsyncInputAdapter::~AsyncInputAdapter()
{
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    if (m_current_id != 0)
    {
      m_interrupt_func(m_current_id);
    }
    m_halt = true;
  }
  m_cv.notify_one();
}

std::unique_ptr<IUserInputFuture> AsyncInputAdapter::AddUserInputRequest(
  const UserInputRequest& request)
{
  sup::dto::uint64 id{0};
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    id = GetNewRequestId();
    m_request_queue.push_back({id, request});
  }
  m_cv.notify_one();
  std::unique_ptr<IUserInputFuture> future{new Future{*this, id}};
  return future;
}

void AsyncInputAdapter::HandleRequestQueue()
{
  auto pred = [this]() {
    return m_halt || !m_request_queue.empty();
  };
  while (true)
  {
    std::unique_lock<std::mutex> lk{m_mtx};
    m_cv.wait(lk, pred);
    if (m_halt)
    {
      return;
    }
    auto req_entry = m_request_queue.front();
    m_request_queue.pop_front();
    m_current_id = req_entry.first;
    lk.unlock();
    // If another thread sees the current id as not zero, this thread must be here:
    auto reply = m_input_func(req_entry.second, req_entry.first);
    lk.lock();
    // If someone has reset the current id, the reply is no longer needed:
    if (m_current_id == req_entry.first)
    {
      m_replies[m_current_id] = reply;
    }
    m_current_id = 0;
  }
}

sup::dto::uint64 AsyncInputAdapter::GetNewRequestId()
{
  // no checks for wrap-around needed here:
  return ++m_last_request_id;
}

bool AsyncInputAdapter::UserInputRequestReady(const Future& token) const
{
  if (!token.IsValid())
  {
    return false;
  }
  std::lock_guard<std::mutex> lk{m_mtx};
  auto reply_it = m_replies.find(token.GetId());
  return reply_it != m_replies.end();
}

UserInputReply AsyncInputAdapter::GetReply(const Future& token)
{
  if (!token.IsValid())
  {
    const std::string error = "AsyncInputAdapter::GetReply(): called with invalid token";
    throw InvalidOperationException{error};
  }
  const auto id = token.GetId();
  std::lock_guard<std::mutex> lk{m_mtx};
  auto reply_it = m_replies.find(token.GetId());
  if (reply_it == m_replies.end())
  {
    const std::string error =
      "AsyncInputAdapter::GetUserInput(): User input with id [" + std::to_string(id) +
      "] unknown or not ready!";
    throw InvalidOperationException{error};
  }
  auto reply = reply_it->second;
  m_replies.erase(reply_it);
  return reply;
}

void AsyncInputAdapter::CancelInputRequest(const Future& token)
{
  if (!token.IsValid())
  {
    return;
  }
  const auto id = token.GetId();
  auto req_pred = [id](const RequestEntry& req){
    return req.first == id;
  };
  std::lock_guard<std::mutex> lk{m_mtx};
  auto request_it = std::find_if(m_request_queue.begin(), m_request_queue.end(), req_pred);
  if (request_it != m_request_queue.end())
  {
    // request is still queued, so removal from the queue is all that is needed
    m_request_queue.erase(request_it);
    return;
  }
  auto reply_it = m_replies.find(id);
  if (reply_it != m_replies.end())
  {
    // reply is already provided, so just removing it is enough
    m_replies.erase(reply_it);
    return;
  }
  if (m_current_id == id)
  {
    // Here, we need to signal the current request to interrupt and to not publish its result
    m_interrupt_func(id);
    m_current_id = 0;
  }
}

AsyncInputAdapter::Future::Future(AsyncInputAdapter& input_handler, sup::dto::uint64 id)
  : m_input_handler{input_handler}
  , m_id{id}
{}

AsyncInputAdapter::Future::~Future()
{
  if (IsValid())
  {
    m_input_handler.CancelInputRequest(*this);
  }
}

sup::dto::uint64 AsyncInputAdapter::Future::GetId() const
{
  return m_id;
}

bool AsyncInputAdapter::Future::IsValid() const
{
  return m_id != 0;
}

bool AsyncInputAdapter::Future::IsReady() const
{
  return m_input_handler.UserInputRequestReady(*this);
}

UserInputReply AsyncInputAdapter::Future::GetValue()
{
  auto result = m_input_handler.GetReply(*this);
  m_id = 0;
  return result;
}

}  // namespace sequencer

}  // namespace sup
