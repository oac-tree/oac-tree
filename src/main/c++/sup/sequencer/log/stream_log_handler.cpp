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

#include "stream_log_handler.h"

namespace sup
{
namespace sequencer
{
namespace log
{
StreamLogHandler::StreamLogHandler(std::ostream& out_stream)
  : m_out_stream{out_stream}
{}

StreamLogHandler::~StreamLogHandler() = default;

void StreamLogHandler::LogMessage(const std::string& message) const
{
  m_out_stream << message << std::endl;
}
}  // namespace log

}  // namespace sequencer

}  // namespace sup
