/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/log_severity.h>

#include <algorithm>
#include <map>

namespace
{
const std::map<int, std::string>& GetSeverityMap();
}

namespace sup
{
namespace oac_tree
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

Severity GetSeverityFromString(const std::string& str)
{
  const auto& sev_map = GetSeverityMap();
  auto on_element = [&str](auto &element) { return str == element.second; };
  auto it = find_if(sev_map.begin(), sev_map.end(), on_element);
  return it == sev_map.end() ? NUMBER_OF_LOG_LEVELS : static_cast<Severity>(it->first);
}

}  // namespace log

}  // namespace oac_tree

}  // namespace sup

namespace
{
using namespace sup::oac_tree::log;
const std::map<int, std::string>& GetSeverityMap()
{
  static std::map<int, std::string> severity_map = {
      {SUP_SEQ_LOG_EMERG, EmergencyString},
      {SUP_SEQ_LOG_ALERT, AlertString},
      {SUP_SEQ_LOG_CRIT, CriticalString},
      {SUP_SEQ_LOG_ERR, ErrorString},
      {SUP_SEQ_LOG_WARNING, WarningString},
      {SUP_SEQ_LOG_NOTICE, NoticeString},
      {SUP_SEQ_LOG_INFO, InfoString},
      {SUP_SEQ_LOG_DEBUG, DebugString},
      {SUP_SEQ_LOG_TRACE, TraceString}};
  return severity_map;
}
}  // namespace
