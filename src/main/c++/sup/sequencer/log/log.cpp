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

#include "log_manager.h"
#include "stream_log_handler.h"

#include <sup/sequencer/log.h>

namespace
{
sup::sequencer::log::LogManager& GlobalLogManager();
void LogMessage(int severity, const std::string& source, const std::string& message);
}

namespace sup
{
namespace sequencer
{
namespace log
{
class LogStreamRedirectorImpl
{
public:
  LogStreamRedirectorImpl(std::ostream& out_stream);
  ~LogStreamRedirectorImpl();
private:
  StreamLogHandler m_stream_handler;
  ILogHandler* m_old_handler;
};

LogStreamRedirector::LogStreamRedirector(std::ostream& out_stream)
  : m_impl{new LogStreamRedirectorImpl(out_stream)}
{}

LogStreamRedirector::~LogStreamRedirector() = default;

LogStreamRedirectorImpl::LogStreamRedirectorImpl(std::ostream& out_stream)
  : m_stream_handler{out_stream}
  , m_old_handler{nullptr}
{
  m_old_handler = GlobalLogManager().SetHandler(&m_stream_handler);
}

LogStreamRedirectorImpl::~LogStreamRedirectorImpl()
{
  GlobalLogManager().SetHandler(m_old_handler);
}

void SetMaxSeverity(int severity)
{
  GlobalLogManager().SetMaxSeverity(severity);
}

void SetStdOut()
{
  GlobalLogManager().SetStdout();
}

void SetSysLog()
{
  GlobalLogManager().SetSyslog();
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
sup::sequencer::log::LogManager& GlobalLogManager()
{
  static sup::sequencer::log::LogManager manager{};
  return manager;
}

void LogMessage(int severity, const std::string& source, const std::string& message)
{
  GlobalLogManager().LogMessage(severity, source, message);
}
}  // unnamed namespace
