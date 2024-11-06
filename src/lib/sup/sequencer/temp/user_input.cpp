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

namespace sup
{
namespace sequencer
{

AsyncUserInput::AsyncUserInput(IUserInput& sync_input)
  : m_sync_input{sync_input}
  , m_future{}
{}

AsyncUserInput::~AsyncUserInput() = default;

bool AsyncUserInput::AddUserInputRequest()
{
  if (m_future.valid())
  {
    return false;
  }
  m_future = std::async(std::launch::async, &IUserInput::GetUserValue, std::addressof(m_sync_input));
  return true;
}

bool AsyncUserInput::UserInputRequestReady() const
{
  if (!m_future.valid())
  {
    return false;
  }
  auto result = m_future.wait_for(std::chrono::seconds(0));
  return result == std::future_status::ready;
}

int AsyncUserInput::GetUserInput()
{
  if (!UserInputRequestReady())
  {
    throw InvalidOperationException{"User input was not ready!"};
  }
  return m_future.get();
}

}  // namespace sequencer

}  // namespace sup
