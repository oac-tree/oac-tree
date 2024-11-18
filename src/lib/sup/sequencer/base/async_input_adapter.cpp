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
  , m_requests{}
  , m_cancelled{}
{
  m_handler_future = std::async(std::launch::async, &AsyncInputAdapter::HandleRequestQueue, this);
}

AsyncInputAdapter::~AsyncInputAdapter()
{
  m_halt.store(true);
  m_cv.notify_one();
  for (auto& request : m_requests)
  {
    m_interrupt_func(request.first);
  }
}

std::unique_ptr<IUserInputFuture> AsyncInputAdapter::AddUserInputRequest(
  const UserInputRequest& request)
{
  std::lock_guard<std::mutex> lk{m_mtx};
  CleanUpUnused();
  auto id = GetNewRequestId();
  auto request_future = std::async(std::launch::async, m_input_func, request, id);
  m_requests.emplace(std::make_pair(id, std::move(request_future)));
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

bool AsyncInputAdapter::UserInputRequestReady(const Future& token) const
{
  if (!token.IsValid())
  {
    return false;
  }
  std::lock_guard<std::mutex> lk{m_mtx};
  auto request_it = m_requests.find(token.GetId());
  if (request_it == m_requests.end())
  {
    return false;
  }
  auto result = request_it->second.wait_for(std::chrono::seconds(0));
  return result == std::future_status::ready;
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
  CleanUpUnused();
  auto request_it = m_requests.find(id);
  if (request_it == m_requests.end())
  {
    const std::string error =
      "AsyncInputAdapter::GetUserInput(): called with unknown token id: " + std::to_string(id);
    throw InvalidOperationException{error};
  }
  auto result = request_it->second.wait_for(std::chrono::seconds(0));
  if (result != std::future_status::ready)
  {
    const std::string error = "User input with id [" + std::to_string(id) + "] was not ready!";
    throw InvalidOperationException{error};
  }
  auto response = request_it->second.get();
  m_requests.erase(request_it);
  return response;
}

void AsyncInputAdapter::CancelInputRequest(const Future& token)
{
  if (!token.IsValid())
  {
    return;
  }
  const auto id = token.GetId();
  std::lock_guard<std::mutex> lk{m_mtx};
  auto request_it = m_requests.find(id);
  if (request_it == m_requests.end())
  {
    return;
  }
  m_interrupt_func(id);
  m_cancelled.push_back(id);
  CleanUpUnused();
}

sup::dto::uint64 AsyncInputAdapter::GetNewRequestId()
{
  ++m_last_request_id;
  while (m_last_request_id == 0 || m_requests.find(m_last_request_id) != m_requests.end())
  {
    ++m_last_request_id;
  }
  return m_last_request_id;
}

void AsyncInputAdapter::CleanUpUnused()
{
  auto it = m_cancelled.begin();
  while (it != m_cancelled.end())
  {
    auto request_it = m_requests.find(*it);
    if (request_it == m_requests.end())
    {
      it = m_cancelled.erase(it);
    }
    else
    {
      auto result = request_it->second.wait_for(std::chrono::seconds(0));
      if (result != std::future_status::timeout)
      {
        m_requests.erase(request_it);
        it = m_cancelled.erase(it);
      }
      else
      {
        ++it;
      }
    }
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
