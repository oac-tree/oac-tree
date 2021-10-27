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

void SimpleLog(int severity, const std::string& source, const std::string& message);

enum class LogOutput
{
  standard_output,
  system_log
};

void SetLogOutput(LogOutput log_output);

template <typename... Args>
void VariadicLog(int severity, const std::string& source, const std::string& format, Args&&... args)
{
  char buffer[BUFFER_SIZE];
  (void)snprintf(buffer, BUFFER_SIZE, format.c_str(), std::forward<Args>(args)...);
  SimpleLog(severity, source, std::string(buffer));
}

template <>
void VariadicLog<>(int severity, const std::string& source, const std::string& message)
{
  SimpleLog(severity, source, message);
}

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_Log_h_
