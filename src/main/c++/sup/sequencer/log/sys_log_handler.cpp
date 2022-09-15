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

#include "sys_log_handler.h"

#include <iostream>

namespace sup
{
namespace sequencer
{
namespace log
{
SysLogHandler::SysLogHandler() = default;

SysLogHandler::~SysLogHandler() = default;

void SysLogHandler::LogMessage(const std::string& message) const
{
  std::cout << message << std::endl;
}
}  // namespace log

}  // namespace sequencer

}  // namespace sup
