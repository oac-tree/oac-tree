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

#ifndef SUP_SEQUENCER_STREAM_LOG_HANDLER_H_
#define SUP_SEQUENCER_STREAM_LOG_HANDLER_H_

#include "i_log_handler.h"

#include <ostream>

namespace sup
{
namespace sequencer
{
namespace log
{
class StreamLogHandler : public ILogHandler
{
public:
  StreamLogHandler(std::ostream& out_stream);
  ~StreamLogHandler();

  void LogMessage(int severity, const std::string& source,
                  const std::string& message) const override;
private:
  std::ostream& m_out_stream;
};
}  // namespace log

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_STREAM_LOG_HANDLER_H_
