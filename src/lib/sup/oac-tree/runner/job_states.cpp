/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
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

#include <sup/oac-tree/job_states.h>

#include <map>
#include <set>

namespace sup
{
namespace oac_tree
{

std::string ToString(JobState state)
{
  static const std::map<JobState, std::string> state_to_string = {
    { JobState::kInitial, "Initial" },
    { JobState::kPaused, "Paused" },
    { JobState::kStepping, "Stepping" },
    { JobState::kRunning, "Running" },
    { JobState::kSucceeded, "Success" },
    { JobState::kFailed, "Failure" },
    { JobState::kHalted, "Halted" }
  };
  auto iter = state_to_string.find(state);
  if (iter == state_to_string.end())
  {
    return "Unknown JobState";
  }
  return iter->second;
}

bool IsFinishedJobState(JobState state)
{
  static const std::set<JobState> finished_states = {
    JobState::kSucceeded,
    JobState::kFailed,
    JobState::kHalted
  };
  return finished_states.find(state) != finished_states.end();
}

}  // namespace oac_tree

}  // namespace sup
