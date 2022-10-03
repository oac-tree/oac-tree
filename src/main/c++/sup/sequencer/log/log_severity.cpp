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

#include "log_severity.h"

#include <sup/sequencer/log.h>

#include <map>

namespace
{
const std::map<int, std::string>& GetSeverityMap();
}

namespace sup
{
namespace sequencer
{
namespace log
{
std::string SeverityString(int severity)
{
  const auto& sev_map = GetSeverityMap();
  auto it = sev_map.find(severity);
  if (it == sev_map.end())
  {
    return "UNKNOWN";
  }
  return it->second;
}

}  // namespace log

}  // namespace sequencer

}  // namespace sup

namespace
{
const std::map<int, std::string>& GetSeverityMap()
{
  static std::map<int, std::string> severity_map = {
    { sup::sequencer::log::SUP_LOG_EMERG, sup::sequencer::log::EmergencyString},
    { sup::sequencer::log::SUP_LOG_ALERT, sup::sequencer::log::AlertString},
    { sup::sequencer::log::SUP_LOG_CRIT, sup::sequencer::log::CriticalString},
    { sup::sequencer::log::SUP_LOG_ERR, sup::sequencer::log::ErrorString},
    { sup::sequencer::log::SUP_LOG_WARNING, sup::sequencer::log::WarningString},
    { sup::sequencer::log::SUP_LOG_NOTICE, sup::sequencer::log::NoticeString},
    { sup::sequencer::log::SUP_LOG_INFO, sup::sequencer::log::InfoString},
    { sup::sequencer::log::SUP_LOG_DEBUG, sup::sequencer::log::DebugString},
    { sup::sequencer::log::SUP_LOG_TRACE, sup::sequencer::log::TraceString}
  };
  return severity_map;
}
}
