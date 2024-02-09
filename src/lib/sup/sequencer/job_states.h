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

#ifndef SUP_SEQUENCER_JOB_STATES_H_
#define SUP_SEQUENCER_JOB_STATES_H_

#include <string>

namespace sup
{
namespace sequencer
{
enum class JobState
{
  kInitial = 0,
  kPaused,
  kStepping,
  kRunning,
  kSucceeded,
  kFailed,
  kHalted
};

std::string ToString(JobState state);

bool IsFinishedJobState(JobState state);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_JOB_STATES_H_
