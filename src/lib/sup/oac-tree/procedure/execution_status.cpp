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

#include <sup/oac-tree/execution_status.h>

namespace sup
{
namespace oac_tree
{

std::string StatusToString(const ExecutionStatus& status)
{
  switch (status)
  {
  case ExecutionStatus::NOT_STARTED:
    return "Not started";
  case ExecutionStatus::NOT_FINISHED:
    return "Not finished";
  case ExecutionStatus::RUNNING:
    return "Running";
  case ExecutionStatus::SUCCESS:
    return "Success";
  case ExecutionStatus::FAILURE:
    return "Failure";
  }
  return "Unknown status";
}

bool IsFinishedStatus(const ExecutionStatus& status)
{
  return status == ExecutionStatus::SUCCESS || status == ExecutionStatus::FAILURE;
}

bool NeedsExecute(const ExecutionStatus& status)
{
  return !IsFinishedStatus(status);
}

bool ReadyForExecute(const ExecutionStatus& status)
{
  return status == ExecutionStatus::NOT_STARTED || status == ExecutionStatus::NOT_FINISHED;
}

bool IsExecutingStatus(const ExecutionStatus& status)
{
  return status == ExecutionStatus::NOT_FINISHED || status == ExecutionStatus::RUNNING;
}

}  // namespace oac_tree

}  // namespace sup
