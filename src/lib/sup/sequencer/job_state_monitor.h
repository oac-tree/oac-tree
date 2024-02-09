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

#ifndef SUP_SEQUENCER_JOB_STATE_MONITOR_H_
#define SUP_SEQUENCER_JOB_STATE_MONITOR_H_

#include <sup/sequencer/job_states.h>

#include <condition_variable>
#include <functional>
#include <mutex>

namespace sup
{
namespace sequencer
{
class JobStateMonitor
{
public:
  JobStateMonitor();
  ~JobStateMonitor();

  std::function<void(JobState)> GetStateCallback();

  JobState WaitForFinished() const;

  bool WaitForState(JobState state, double seconds) const;

private:
  void OnStateChange(JobState state);
  JobState m_state;
  mutable std::mutex m_mtx;
  mutable std::condition_variable m_cv;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_STATE_MONITOR_H_
