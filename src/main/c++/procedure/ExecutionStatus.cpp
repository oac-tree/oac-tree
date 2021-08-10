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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files

#include <common/log-api.h>

// Local header files

#include "ExecutionStatus.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

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

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
