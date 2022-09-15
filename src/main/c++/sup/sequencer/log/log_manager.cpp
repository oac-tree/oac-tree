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
{}

LogManager::~LogManager() = default;

void LogManager::LogMessage(const std::string& message) const
{
  auto handler = m_handler.load();
  handler->LogMessage(message);
}

ILogHandler* LogManager::SetHandler(ILogHandler* handler)
{
  return m_handler.exchange(handler);
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

