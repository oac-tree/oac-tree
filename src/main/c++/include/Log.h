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
 * Copyright (c) : 2010-2021 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/**
 * @file Log.h
 * @brief Header file for logging.
 * @date 27/10/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the logging functions.
 */

#ifndef _SEQ_Log_h_
#define _SEQ_Log_h_

#include <string>
#include <cstdarg>
#include <stdio.h>

namespace sup
{
namespace sequencer
{
constexpr size_t BUFFER_SIZE = 1024;

class ILogger
{
public:
  enum LogSeverity
  {
    SUP_LOG_EMERG = 0,
    SUP_LOG_ALERT,
    SUP_LOG_CRIT,
    SUP_LOG_ERR,
    SUP_LOG_WARNING,
    SUP_LOG_NOTICE,
    SUP_LOG_INFO,
    SUP_LOG_DEBUG,
    SUP_LOG_TRACE,
    NUMBER_OF_LOG_LEVELS
  };
  static std::string SeverityToString(int severity);

  virtual ~ILogger() = default;

  void SetMaxSeverity(int severity);
  int MaxSeverity() const;

  template <typename... Args> void Emergency(const std::string& source,
                                             const std::string& format, Args&&... args);
  template <typename... Args> void Alert(const std::string& source,
                                         const std::string& format, Args&&... args);
  template <typename... Args> void Critical(const std::string& source,
                                            const std::string& format, Args&&... args);
  template <typename... Args> void Error(const std::string& source,
                                         const std::string& format, Args&&... args);
  template <typename... Args> void Warning(const std::string& source,
                                           const std::string& format, Args&&... args);
  template <typename... Args> void Notice(const std::string& source,
                                          const std::string& format, Args&&... args);
  template <typename... Args> void Info(const std::string& source,
                                        const std::string& format, Args&&... args);
  template <typename... Args> void Debug(const std::string& source,
                                         const std::string& format, Args&&... args);
  template <typename... Args> void Trace(const std::string& source,
                                         const std::string& format, Args&&... args);

private:
  int max_severity;
  virtual void SimpleLog(int severity, const std::string& source, const std::string& message) = 0;

  template <typename... Args> void VariadicLog(int severity, const std::string& source,
                                               const std::string& format, Args&&... args);
};

template <typename... Args>
void ILogger::VariadicLog(int severity, const std::string& source,
                          const std::string& format, Args&&... args)
{
  if (severity > MaxSeverity())
  {
    return;
  }
  char buffer[BUFFER_SIZE];
  (void)snprintf(buffer, BUFFER_SIZE, format.c_str(), std::forward<Args>(args)...);
  SimpleLog(severity, source, std::string(buffer));
}

template <>
void ILogger::VariadicLog<>(int severity, const std::string& source, const std::string& message)
{
  if (severity > MaxSeverity())
  {
    return;
  }
  SimpleLog(severity, source, message);
}

template <typename... Args>
void ILogger::Emergency(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_EMERG, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Alert(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_ALERT, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Critical(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_CRIT, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Error(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_ERR, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Warning(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_WARNING, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Notice(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_NOTICE, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Info(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_INFO, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Debug(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_DEBUG, source, format, std::forward<Args>(args)...);
}

template <typename... Args>
void ILogger::Trace(const std::string& source, const std::string& format, Args&&... args)
{
  VariadicLog(SUP_LOG_TRACE, source, format, std::forward<Args>(args)...);
}

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_Log_h_
