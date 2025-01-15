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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "daemon_interface.h"

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/log_severity.h>

#include <sup/dto/anyvalue_helper.h>

#include <map>

namespace
{
using LogMemberFunctionType = void (sup::log::DefaultLogger::*)(const std::string&) const;
const std::map<int, LogMemberFunctionType>& LogMemberFunctionMap();

}  // unnamed namespace

namespace sup
{
namespace sequencer
{
DaemonInterface::DaemonInterface(const sup::log::DefaultLogger& logger)
  : m_logger{logger}
{}

DaemonInterface::~DaemonInterface() = default;

void DaemonInterface::UpdateInstructionStatus(const Instruction *instruction)
{
  std::string info_message = "Instruction (" + instruction->GetType() + ":" +
    instruction->GetName() + ") : " + StatusToString(instruction->GetStatus());
  m_logger.Info(info_message);
}

void DaemonInterface::VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                                      bool connected)
{}

bool DaemonInterface::PutValue(const sup::dto::AnyValue &value, const std::string &description)
{
  std::string json_rep = sup::dto::ValuesToJSONString(value);
  std::string info_message = description + " (" + value.GetTypeName() + "):" + json_rep;
  m_logger.Info(info_message);
  return true;
}

std::unique_ptr<IUserInputFuture> DaemonInterface::RequestUserInput(const UserInputRequest& request)
{
  (void)request;
  std::string error_message = "DaemonInterface::RequestUserInput(): is not implemented";
  m_logger.Error(error_message);
  return std::make_unique<UnsupportedInputFuture>();
}

void DaemonInterface::Message(const std::string& message)
{
  m_logger.Info(message);
}

void DaemonInterface::Log(int severity, const std::string& message)
{
  auto& mem_func_map = LogMemberFunctionMap();
  auto it = mem_func_map.find(severity);
  if (it == mem_func_map.end())
  {
    return;
  }
  LogMemberFunctionType mem_func = it->second;
  (m_logger.*mem_func)(message);
}

}  // namespace sequencer

}  // namespace sup

namespace
{
using namespace sup::sequencer::log;
const std::map<int, LogMemberFunctionType>& LogMemberFunctionMap()
{
  static std::map<int, LogMemberFunctionType> mem_func_map = {
    { SUP_SEQ_LOG_EMERG, &sup::log::DefaultLogger::Emergency },
    { SUP_SEQ_LOG_ALERT, &sup::log::DefaultLogger::Alert },
    { SUP_SEQ_LOG_CRIT, &sup::log::DefaultLogger::Critical },
    { SUP_SEQ_LOG_ERR, &sup::log::DefaultLogger::Error },
    { SUP_SEQ_LOG_WARNING, &sup::log::DefaultLogger::Warning },
    { SUP_SEQ_LOG_NOTICE, &sup::log::DefaultLogger::Notice },
    { SUP_SEQ_LOG_INFO, &sup::log::DefaultLogger::Info },
    { SUP_SEQ_LOG_DEBUG, &sup::log::DefaultLogger::Debug },
    { SUP_SEQ_LOG_TRACE, &sup::log::DefaultLogger::Trace }
  };
  return mem_func_map;
}
}  // unnamed namespace
