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

namespace sup
{
namespace sequencer
{
/**
 * @brief Pure interface for objects that can receive JobState updates from JobController.
 *
 */
class JobStateMonitor
{
public:
  ~JobStateMonitor();

  virtual void OnStateChange(JobState state) = 0;

  virtual JobState GetCurrentState() const = 0;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_STATE_MONITOR_H_
