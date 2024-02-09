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

#include <sup/sequencer/job_state_monitor.h>

#include <chrono>
#include <cmath>

namespace sup
{
namespace sequencer
{
JobStateMonitor::JobStateMonitor()
  : m_state{JobState::kInitial}
  , m_mtx{}
  , m_cv{}
{}

JobStateMonitor::~JobStateMonitor() = default;

std::function<void(JobState)> JobStateMonitor::GetStateCallback()
{
  return [this](JobState state){
    OnStateChange(state);
  };
}

JobState JobStateMonitor::WaitForFinished() const
{
  auto pred = [this](){
    return IsFinishedJobState(m_state);
  };
  std::unique_lock<std::mutex> lk{m_mtx};
  m_cv.wait(lk, pred);
  return m_state;
}

bool JobStateMonitor::WaitForState(JobState state, double seconds) const
{
  auto duration = std::chrono::nanoseconds(std::lround(seconds * 1e9));
  std::unique_lock<std::mutex> lk{m_mtx};
  return m_cv.wait_for(lk, duration, [this, state](){
    return m_state == state;
  });
}

void JobStateMonitor::OnStateChange(JobState state)
{
  {
    std::lock_guard<std::mutex> lk{m_mtx};
    m_state = state;
  }
  m_cv.notify_one();
}

}  // namespace sequencer

}  // namespace sup
