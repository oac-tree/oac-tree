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

#include "stdout_log_handler.h"
#include "sys_log_handler.h"

#include <sup/sequencer/log.h>

#include <algorithm>

namespace
{
sup::sequencer::log::ILogHandler* GetStdoutLogHandler();
sup::sequencer::log::ILogHandler* GetSysLogHandler();
}

namespace sup
{
namespace sequencer
{
namespace log
{
LogManager::LogManager()
  : m_handler{GetSysLogHandler()}
  , m_max_severity{SUP_LOG_DEBUG}
{}

LogManager::~LogManager() = default;

void LogManager::LogMessage(int severity, const std::string& source, const std::string& message) const
{
  if (severity > m_max_severity.load())
  {
    return;
  }
  auto handler = m_handler.load();
  handler->LogMessage(severity, source, message);
}

ILogHandler* LogManager::SetHandler(ILogHandler* handler)
{
  return m_handler.exchange(handler);
}

void LogManager::SetMaxSeverity(int max_severity)
{
  auto positive_bound = std::max(max_severity, 0);
  auto bounded_max = std::min(positive_bound, NUMBER_OF_LOG_LEVELS - 1);
  m_max_severity.store(bounded_max);
}

void LogManager::SetStdout()
{
  SetHandler(GetStdoutLogHandler());
}

void LogManager::SetSyslog()
{
  SetHandler(GetSysLogHandler());
}

}  // namespace log

}  // namespace sequencer

}  // namespace sup

namespace
{
sup::sequencer::log::ILogHandler* GetStdoutLogHandler()
{
  static sup::sequencer::log::StdoutLogHandler handler;
  return &handler;
}

sup::sequencer::log::ILogHandler* GetSysLogHandler()
{
  static sup::sequencer::log::SysLogHandler handler;
  return &handler;
}
}

