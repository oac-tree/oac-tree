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
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sup/sequencer/constants.h>
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

LogInstruction::LogInstruction()
  : Instruction(Type)
{
  AddAttributeDefinition(Constants::MESSAGE_ATTRIBUTE_NAME).SetCategory(AttributeCategory::kBoth);
  AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName);
  AddAttributeDefinition(Constants::SEVERITY_ATTRIBUTE_NAME).SetCategory(AttributeCategory::kBoth);
  AddConstraint(
    MakeConstraint<Or>(MakeConstraint<Exists>(Constants::MESSAGE_ATTRIBUTE_NAME),
                       MakeConstraint<Exists>(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)));
}

LogInstruction::~LogInstruction() = default;

ExecutionStatus LogInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  std::string severity_str = "info";
  if (!GetAttributeValueAs(Constants::SEVERITY_ATTRIBUTE_NAME, ws, ui, severity_str))
  {
    return ExecutionStatus::FAILURE;
  }
  auto severity = SeverityFromString(severity_str);
  if (severity < 0)
  {
    std::string error_message = InstructionErrorProlog(*this) +
      "could not parse severity [" + severity_str + "] as valid severity level";
    LogError(ui, error_message);
    return ExecutionStatus::FAILURE;
  }
  std::string message;
  if (!GetAttributeValueAs(Constants::MESSAGE_ATTRIBUTE_NAME, ws, ui, message))
  {
    return ExecutionStatus::FAILURE;
  }
  std::ostringstream oss;
  oss << message;
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!sup::dto::IsEmptyValue(value))
  {
    oss << sup::dto::ValuesToJSONString(value);
  }
  ui.Log(severity, oss.str());
  return ExecutionStatus::SUCCESS;
}

} // namespace sequencer

} // namespace sup

namespace
{
using namespace sup::sequencer::log;
int SeverityFromString(const std::string& level)
{
  static const std::map<std::string, int> string_severity_map = {
      {"emergency", SUP_SEQ_LOG_EMERG},
      {"alert", SUP_SEQ_LOG_ALERT},
      {"critical", SUP_SEQ_LOG_CRIT},
      {"error", SUP_SEQ_LOG_ERR},
      {"warning", SUP_SEQ_LOG_WARNING},
      {"notice", SUP_SEQ_LOG_NOTICE},
      {"info", SUP_SEQ_LOG_INFO},
      {"debug", SUP_SEQ_LOG_DEBUG},
      {"trace", SUP_SEQ_LOG_TRACE},
    };
  auto it = string_severity_map.find(level);
  if (it == string_severity_map.end())
  {
    return -1;
  }
  return it->second;
}

}  // unnamed namespace
