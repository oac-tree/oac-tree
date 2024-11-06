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

#include "user_input.h"

#include <sup/sequencer/exceptions.h>

#include <chrono>
#include <utility>

namespace sup
{
namespace sequencer
{

AsyncUserInput::AsyncUserInput(IUserInput& sync_input)
  : m_sync_input{sync_input}
  , m_requests{}
  , m_last_request_id{0}
{}

AsyncUserInput::~AsyncUserInput()
{
  for (auto& request : m_requests)
  {
    m_sync_input.Interrupt(request.first);
  }
}

sup::dto::uint64 AsyncUserInput::AddUserInputRequest()
{
  auto id = GetNewRequestId();
  auto request_future =
    std::async(std::launch::async, &IUserInput::GetUserValue, std::addressof(m_sync_input), id);
  m_requests.emplace(std::make_pair(id, std::move(request_future)));
  return id;
}

bool AsyncUserInput::UserInputRequestReady(sup::dto::uint64 id) const
{
  auto request_it = m_requests.find(id);
  if (request_it == m_requests.end())
  {
    return false;
  }
  auto result = request_it->second.wait_for(std::chrono::seconds(0));
  return result == std::future_status::ready;
}

int AsyncUserInput::GetUserInput(sup::dto::uint64 id)
{
  auto request_it = m_requests.find(id);
  if (request_it == m_requests.end())
  {
    return false;
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

sup::dto::uint64 AsyncUserInput::GetNewRequestId()
{
  ++m_last_request_id;
  while (m_last_request_id == 0 || m_requests.find(m_last_request_id) != m_requests.end())
  {
    ++m_last_request_id;
  }
  return m_last_request_id;
}

}  // namespace sequencer

}  // namespace sup
