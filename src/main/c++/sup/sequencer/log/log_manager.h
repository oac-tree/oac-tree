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

#ifndef SUP_SEQUENCER_LOG_MANAGER_H_
#define SUP_SEQUENCER_LOG_MANAGER_H_

#include "i_log_handler.h"

#include <atomic>

namespace sup
{
namespace sequencer
{
namespace log
{
class LogManager
{
public:
  LogManager();
  ~LogManager();

  void LogMessage(int severity, const std::string& source, const std::string& message) const;
  ILogHandler* SetHandler(ILogHandler* handler);
  void SetMaxSeverity(int max_severity);
  void SetStdout();
  void SetSyslog();
private:
  std::atomic<ILogHandler*> m_handler;
  std::atomic_int m_max_severity;
};

}  // namespace log

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOG_MANAGER_H_
