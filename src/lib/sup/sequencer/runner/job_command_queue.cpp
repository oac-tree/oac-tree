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

#include <sup/sequencer/job_command_queue.h>

namespace sup
{
namespace sequencer
{

JobCommandQueue::JobCommandQueue()
  : m_command_queue{}
  , m_mtx{}
  , m_cv{}
{}

JobCommandQueue::~JobCommandQueue() = default;

void JobCommandQueue::Push(JobCommand command)
{
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    m_command_queue.push_back(command);
  }
  m_cv.notify_one();
}

bool JobCommandQueue::PriorityPush(JobCommand command, std::function<void()> func)
{
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    if (!m_command_queue.empty() && command <= m_command_queue.front())
    {
      return false;
    }
    if (func)
    {
      func();
    }
    m_command_queue.push_front(command);
  }
  m_cv.notify_one();
  return true;
}

bool JobCommandQueue::IsEmpty() const
{
  std::lock_guard<std::mutex> lk{m_mtx};
  return m_command_queue.empty();
}

std::size_t JobCommandQueue::GetSize() const
{
  std::lock_guard<std::mutex> lk{m_mtx};
  return m_command_queue.size();
}

bool JobCommandQueue::TryPop(JobCommand& command)
{
  std::lock_guard<std::mutex> lk{m_mtx};
  if (m_command_queue.empty())
  {
    return false;
  }
  command = m_command_queue.front();
  m_command_queue.pop_front();
  return true;
}

JobCommand JobCommandQueue::WaitForNextCommand()
{
  std::unique_lock<std::mutex> lk{m_mtx};
  m_cv.wait(lk, [this]{ return !m_command_queue.empty(); });
  auto command = m_command_queue.front();
  m_command_queue.pop_front();
  return command;
}

}  // namespace sequencer

}  // namespace sup
