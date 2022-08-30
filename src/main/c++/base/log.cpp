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

#include "log.h"

#include <common/log-api.h>

namespace
{
void LogMessage(int severity, const std::string& source, const std::string& message);
}

namespace sup
{
namespace sequencer
{
namespace log
{

void SetMaxSeverity(int severity)
{
  ccs::log::SetFilter(severity);
}

void SetStdOut()
{
  (void)ccs::log::SetStdout();
}

void SetSysLog()
{
  (void)ccs::log::SetSyslog();
}

void SimpleEmergency(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_EMERG, source, message);
}

void SimpleAlert(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_ALERT, source, message);
}

void SimpleCritical(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_CRIT, source, message);
}

void SimpleError(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_ERR, source, message);
}

void SimpleWarning(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_WARNING, source, message);
}

void SimpleNotice(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_NOTICE, source, message);
}

void SimpleInfo(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_INFO, source, message);
}

void SimpleDebug(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_DEBUG, source, message);
}

void SimpleTrace(const std::string& source, const std::string& message)
{
  LogMessage(SUP_LOG_TRACE, source, message);
}

}  // namespace log

}  // namespace sequencer

}  // namespace sup

namespace
{
void LogMessage(int severity, const std::string& source, const std::string& message)
{
  ccs::log::Message(severity, source.c_str(), message.c_str());
}
}
