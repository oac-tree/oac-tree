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
class Instruction;
/**
 * @brief Pure interface for objects that can receive JobState updates from JobController.
 *
 * @note The member functions are all declared noexcept as implementations should not throw.
 *
 */
class JobStateMonitor
{
public:
  virtual ~JobStateMonitor();

  virtual void OnStateChange(JobState state) noexcept = 0;

  virtual void OnBreakpointChange(const Instruction* instruction, bool breakpoint_set) noexcept = 0;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_STATE_MONITOR_H_
