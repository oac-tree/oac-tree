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

#ifndef SUP_SEQUENCER_LOG_SEVERITY_H_
#define SUP_SEQUENCER_LOG_SEVERITY_H_

#include <string>

namespace sup
{
namespace sequencer
{
namespace log
{

/**
 * Severity levels are defined according to the usual ones from the Syslog protocol to ease
 * forwarding of log messages to the syslog.
 *
 * All levels lower than SUP_SEQ_LOG_ERR should at least be accompanied by an exception being
 * thrown. The advice is not to use these at all for logging and rely on exception handling instead.
 *
 * SUP_SEQ_LOG_ERR is used to indicate normal execution (e.g. of an instruction) is not possible.
 * For example, a Copy instruction might not find the required variables in the workspace, or their
 * types are not compatible.
 *
 * SUP_SEQ_LOG_WARNING is used to indicate a milder condition. For example,instruction execution is
 * not proceeding along the main execution branch, but this could be intended behaviour by the
 * designer of the procedure.
 * For example, an instruction might fail to read a variable from the workspace that represents a
 * network variable that is not connected. The aim of the instruction might have been to check
 * connectivity of the variable, so this is not an error condition.
 *
 * All levels higher than SUP_SEQ_LOG_WARNING are informational only and are advised not to be used.
*/
enum Severity
{
  SUP_SEQ_LOG_EMERG = 0,
  SUP_SEQ_LOG_ALERT,
  SUP_SEQ_LOG_CRIT,
  SUP_SEQ_LOG_ERR,
  SUP_SEQ_LOG_WARNING,
  SUP_SEQ_LOG_NOTICE,
  SUP_SEQ_LOG_INFO,
  SUP_SEQ_LOG_DEBUG,
  SUP_SEQ_LOG_TRACE,
  NUMBER_OF_LOG_LEVELS
};

/**
 * @brief Retrieve a string representation of the given severity level.
 */
std::string SeverityString(int severity);

}  // namespace log

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOG_SEVERITY_H_
