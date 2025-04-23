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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_JOB_STATES_H_
#define SUP_OAC_TREE_JOB_STATES_H_

#include <string>

namespace sup
{
namespace oac_tree
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

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_JOB_STATES_H_
