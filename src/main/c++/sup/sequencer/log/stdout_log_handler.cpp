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

#include "stdout_log_handler.h"

#include <sup/sequencer/log_severity.h>

#include <iostream>
#include <sstream>

namespace sup
{
namespace sequencer
{
namespace log
{
StdoutLogHandler::StdoutLogHandler() = default;

StdoutLogHandler::~StdoutLogHandler() = default;

void StdoutLogHandler::LogMessage(int severity, const std::string& source,
                                  const std::string& message) const
{
  std::ostringstream oss;
  oss << "[" << SeverityString(severity) << "] ";
  oss << message;
  if (!source.empty())
  {
    oss << " [" << source << "]";
  }
  oss << std::endl;
  std::cout << oss.str();
}
}  // namespace log

}  // namespace sequencer

}  // namespace sup
