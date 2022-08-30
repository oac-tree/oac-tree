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

/**
 * @file Log.h
 * @brief Header file for logging.
 * @date 27/10/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the logging functions.
 */

#ifndef SUP_SEQUENCER_LOG_H_
#define SUP_SEQUENCER_LOG_H_

#include <string>
#include <cstdarg>
#include <stdio.h>

namespace sup
{
namespace sequencer
{
namespace log
{
constexpr size_t kBufferSize = 1024;
const std::string kSource = "sup::sequencer";

enum Severity
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

template <>
std::string FormatMessage<>(const std::string& message)
{
  return message;
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

template <typename... Args>
void Debug(const std::string& format, Args&&... args)
{
  SimpleDebug(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

template <typename... Args>
void Trace(const std::string& format, Args&&... args)
{
  SimpleTrace(kSource, FormatMessage(format, std::forward<Args>(args)...));
}

}  // namespace log

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOG_H_
