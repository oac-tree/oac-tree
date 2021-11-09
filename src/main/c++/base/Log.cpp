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

#include "Log.h"
#include <common/log-api.h>

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace
{
int& GetMaxSeverity()
{
  static int max_severity = sup::sequencer::SUP_LOG_INFO;
  return max_severity;
}
}

namespace sup
{
namespace sequencer
{

void SetLogOutput(LogOutput log_output)
{
  switch (log_output)
  {
    case LogOutput::standard_output: ccs::log::SetStdout();
                                     break;
    case LogOutput::system_log:      ccs::log::SetSyslog();
                                     break;
  }
}

void SetMaxSeverity(int severity)
{
  auto& max_severity = GetMaxSeverity();
  max_severity = severity;
}

int MaxSeverity()
{
  return GetMaxSeverity();
}

void SimpleLog(int severity, const std::string& source, const std::string& message)
{
  ccs::log::Message(severity, source.c_str(), message.c_str());
}

}  // namespace sequencer

}  // namespace sup

#undef LOG_ALTERN_SRC
