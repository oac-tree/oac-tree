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

#include "Log.h"

namespace sup
{
namespace sequencer
{

std::string ILogger::SeverityToString(int severity)
{
  std::string result;
  switch (severity)
  {
    case SUP_LOG_EMERG:
      result = "emergency";
      break;
    case SUP_LOG_ALERT:
      result = "alert";
      break;
    case SUP_LOG_CRIT:
      result = "critical";
      break;
    case SUP_LOG_ERR:
      result = "error";
      break;
    case SUP_LOG_WARNING:
      result = "warning";
      break;
    case SUP_LOG_NOTICE:
      result = "notice";
      break;
    case SUP_LOG_INFO:
      result = "info";
      break;
    case SUP_LOG_DEBUG:
      result = "debug";
      break;
    case SUP_LOG_TRACE:
      result = "trace";
      break;
  }
  return result;
}

void ILogger::SetMaxSeverity(int severity)
{
  max_severity = severity;
}

int ILogger::MaxSeverity() const
{
  return max_severity;
}

}  // namespace sequencer

}  // namespace sup

#undef LOG_ALTERN_SRC
