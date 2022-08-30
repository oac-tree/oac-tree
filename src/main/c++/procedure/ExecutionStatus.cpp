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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "ExecutionStatus.h"

#include "log.h"

namespace sup
{
namespace sequencer
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

}  // namespace sequencer

}  // namespace sup
