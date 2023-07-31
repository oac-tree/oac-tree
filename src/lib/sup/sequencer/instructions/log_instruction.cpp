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

#include "log_instruction.h"

#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/log_severity.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>

#include <functional>
#include <map>
#include <sstream>

namespace
{
int SeverityFromString(const std::string& level);
}  // unnamed namespace

namespace sup {

namespace sequencer {

const std::string LogInstruction::Type = "Log";

const std::string LOG_SOURCE = "sup::sequencer";

const std::string MESSAGE_ATTRIBUTE_NAME = "message";
const std::string INPUT_ATTRIBUTE_NAME = "input";
const std::string SEVERITY_ATTRIBUTE_NAME = "severity";

LogInstruction::LogInstruction()
  : Instruction(Type)
{
  AddAttributeDefinition(MESSAGE_ATTRIBUTE_NAME, sup::dto::StringType);
  AddAttributeDefinition(INPUT_ATTRIBUTE_NAME, sup::dto::StringType);
  AddAttributeDefinition(SEVERITY_ATTRIBUTE_NAME, sup::dto::StringType);
  AddConstraint(MakeConstraint<Or>(MakeConstraint<Exists>(MESSAGE_ATTRIBUTE_NAME),
                                   MakeConstraint<Exists>(INPUT_ATTRIBUTE_NAME)));
}

LogInstruction::~LogInstruction() = default;

ExecutionStatus LogInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  int severity = log::SUP_SEQ_LOG_INFO;  // Default severity if not explicitly specified
  if (HasAttribute(SEVERITY_ATTRIBUTE_NAME))
  {
    auto severity_str = GetAttributeValue<std::string>(SEVERITY_ATTRIBUTE_NAME);
    severity = SeverityFromString(severity_str);
    if (severity < 0)
    {
      std::string error_message = InstructionErrorProlog(*this) +
        "could not parse severity [" + severity_str + "] as valid severity level";
      ui.LogError(error_message);
      return ExecutionStatus::FAILURE;
    }
  }
  std::ostringstream oss;
  if (HasAttribute(MESSAGE_ATTRIBUTE_NAME))
  {
    oss << GetAttributeValue<std::string>(MESSAGE_ATTRIBUTE_NAME);
  }
  if (HasAttribute(INPUT_ATTRIBUTE_NAME))
  {
    sup::dto::AnyValue value;
    if (!GetValueFromAttributeName(*this, ws, ui, INPUT_ATTRIBUTE_NAME, value))
    {
      return ExecutionStatus::FAILURE;
    }
    oss << sup::dto::ValuesToJSONString(value);
  }
  ui.Log(severity, oss.str());
  return ExecutionStatus::SUCCESS;
}

} // namespace sequencer

} // namespace sup

namespace
{
int SeverityFromString(const std::string& level)
{
  static const std::map<std::string, int> string_severity_map = {
      {"emergency", sup::sequencer::log::SUP_SEQ_LOG_EMERG},
      {"alert", sup::sequencer::log::SUP_SEQ_LOG_ALERT},
      {"critical", sup::sequencer::log::SUP_SEQ_LOG_CRIT},
      {"error", sup::sequencer::log::SUP_SEQ_LOG_ERR},
      {"warning", sup::sequencer::log::SUP_SEQ_LOG_WARNING},
      {"notice", sup::sequencer::log::SUP_SEQ_LOG_NOTICE},
      {"info", sup::sequencer::log::SUP_SEQ_LOG_INFO},
      {"debug", sup::sequencer::log::SUP_SEQ_LOG_DEBUG},
      {"trace", sup::sequencer::log::SUP_SEQ_LOG_TRACE},
    };
  auto it = string_severity_map.find(level);
  if (it == string_severity_map.end())
  {
    return -1;
  }
  return it->second;
}

}  // unnamed namespace
