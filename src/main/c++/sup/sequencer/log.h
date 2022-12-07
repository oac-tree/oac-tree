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

#ifndef SUP_SEQUENCER_LOG_H_
#define SUP_SEQUENCER_LOG_H_

#include <memory>
#include <ostream>
#include <stdio.h>
#include <string>

namespace sup
{
namespace sequencer
{
namespace log
{
constexpr size_t kBufferSize = 1024;
const std::string kSource = "sup::sequencer";

class LogStreamRedirectorImpl;

/**
 * @brief Temporarily redirects log output to the provided stream.
 *
 * @note Upon destruction, the previous log handler will be restored.
 * It is NOT safe to change the log output during the lifetime of this object!
 */
class LogStreamRedirector
{
public:
  LogStreamRedirector(std::ostream& out_stream);
  ~LogStreamRedirector();
private:
  std::unique_ptr<LogStreamRedirectorImpl> m_impl;
};

void SetMaxSeverity(int severity);
void SetStdOut();
void SetSysLog();

void SimpleEmergency(const std::string& source, const std::string& message);
void SimpleAlert(const std::string& source, const std::string& message);
void SimpleCritical(const std::string& source, const std::string& message);
void SimpleError(const std::string& source, const std::string& message);
void SimpleWarning(const std::string& source, const std::string& message);
void SimpleNotice(const std::string& source, const std::string& message);
void SimpleInfo(const std::string& source, const std::string& message);
void SimpleDebug(const std::string& source, const std::string& message);
void SimpleTrace(const std::string& source, const std::string& message);

template <typename... Args>
std::string FormatMessage(const std::string& format, Args&&... args);
template <typename... Args>
void Emergency(const std::string& format, Args&&... args);
template <typename... Args>
void Alert(const std::string& format, Args&&... args);
template <typename... Args>
void Critical(const std::string& format, Args&&... args);
template <typename... Args>
void Error(const std::string& format, Args&&... args);
template <typename... Args>
void Warning(const std::string& format, Args&&... args);
template <typename... Args>
void Notice(const std::string& format, Args&&... args);
template <typename... Args>
void Info(const std::string& format, Args&&... args);
template <typename... Args>
void Debug(const std::string& format, Args&&... args);
template <typename... Args>
void Trace(const std::string& format, Args&&... args);

template <typename... Args>
std::string FormatMessage(const std::string& format, Args&&... args)
{
  char buffer[kBufferSize];
  (void)snprintf(buffer, kBufferSize, format.c_str(), std::forward<Args>(args)...);
  return std::string(buffer);
}

template <typename... Args>
void Emergency(const std::string& format, Args&&... args)
{
  SimpleEmergency(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Alert(const std::string& format, Args&&... args)
{
  SimpleAlert(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Critical(const std::string& format, Args&&... args)
{
  SimpleCritical(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Error(const std::string& format, Args&&... args)
{
  SimpleError(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Warning(const std::string& format, Args&&... args)
{
  SimpleWarning(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Notice(const std::string& format, Args&&... args)
{
  SimpleNotice(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Info(const std::string& format, Args&&... args)
{
  SimpleInfo(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

#ifdef LOG_DEBUG_ENABLE
template <typename... Args>
void Debug(const std::string& format, Args&&... args)
{
  SimpleDebug(kSource, FormatMessage(format, std::forward<Args>(args)...));
}
#else
template <typename... Args>
void Debug(const std::string& format, Args&&... args) {}
#endif

#ifdef LOG_TRACE_ENABLE
template <typename... Args>
void Trace(const std::string& format, Args&&... args)
{
  SimpleTrace(kSource, FormatMessage(format, std::forward<Args>(args)...));
}
#else
template <typename... Args>
void Trace(const std::string& format, Args&&... args) {}
#endif

}  // namespace log

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOG_H_
