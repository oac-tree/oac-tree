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

#include "cl_interface.h"

#include <sup/sequencer/log_severity.h>
#include <sup/sequencer/instruction.h>

#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue_helper.h>

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>

static bool ParseStringToScalarAnyvalue(sup::dto::AnyValue &value, const std::string &str);

namespace sup
{
namespace sequencer
{
using namespace std::placeholders;

CLInterface::CLInterface(const sup::log::BasicLogger& logger)
  : m_logger{logger}
  , m_input_adapter{std::bind(&CLInterface::UserInput, this, _1, _2),
                    std::bind(&CLInterface::Interrupt, this, _1)}
  , m_mtx{}
{}

CLInterface::~CLInterface() = default;

void CLInterface::UpdateInstructionStatus(const Instruction *instruction)
{
  std::string info_message = "Instruction (" + instruction->GetType() + ":" +
    instruction->GetName() + ") : " + StatusToString(instruction->GetStatus());
  m_logger.LogMessage(log::SUP_SEQ_LOG_INFO, info_message);
}

void CLInterface::VariableUpdated(const std::string& name, const sup::dto::AnyValue& value,
                                      bool connected)
{
  std::string info_message;
  if (connected)
  {
    std::string json_rep = sup::dto::ValuesToJSONString(value);
    info_message = "Variable (" + name + ") updated:" + json_rep;
  }
  else
  {
    info_message = "Variable (" + name + ") disconnected";
  }
  m_logger.LogMessage(log::SUP_SEQ_LOG_INFO, info_message);
}

bool CLInterface::PutValue(const sup::dto::AnyValue &value, const std::string &description)
{
  if (!description.empty())
  {
    std::cout << description << std::endl;
  }
  std::string val_str = sup::dto::PrintAnyValue(value);
  if (val_str.empty())
  {
    return false;
  }
  std::cout << val_str << std::endl;
  return true;
}

std::unique_ptr<IUserInputFuture> CLInterface::RequestUserInput(const UserInputRequest& request)
{
  return m_input_adapter.AddUserInputRequest(request);
}

void CLInterface::Message(const std::string& message)
{
  std::cout << message << std::endl;
}

void CLInterface::Log(int severity, const std::string& message)
{
  m_logger.LogMessage(severity, message);
}

UserInputReply CLInterface::UserInput(const UserInputRequest& request, sup::dto::uint64 id)
{
  switch (request.m_request_type)
  {
  case InputRequestType::kUserValue:
  {
    auto failure = CreateUserValueReply(false, {});
    sup::dto::AnyValue value{};
    std::string description{};
    if (!ParseUserValueRequest(request, value, description))
    {
      return failure;
    }
    if (!GetUserValue(value, description))
    {
      return failure;
    }
    return CreateUserValueReply(true, value);
  }
  case InputRequestType::kUserChoice:
  {
    auto failure = CreateUserChoiceReply(false, -1);
    std::vector<std::string> options{};
    sup::dto::AnyValue metadata{};
    if (!ParseUserChoiceRequest(request, options, metadata))
    {
      return failure;
    }
    auto choice = GetUserChoice(options, metadata);
    if (choice < 0)
    {
      return failure;
    }
    return CreateUserChoiceReply(true, choice);
  }
  default:
    break;
  }
  return CreateUserValueReply(false, {});
}

void CLInterface::Interrupt(sup::dto::uint64 id)
{
  (void)id;
}

bool CLInterface::GetUserValue(sup::dto::AnyValue &value, const std::string &description)
{
  std::lock_guard<std::mutex> lk{m_mtx};
  if (!sup::dto::IsScalarValue(value))
  {
    auto json_type = sup::dto::AnyTypeToJSONString(value.GetType());
    std::string error_message =
      "CLInterface::GetUserValue(): only scalar values are supported, "
      "requested type was [" + json_type + "]";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return false;
  }
  std::cout << description << " (" << value.GetTypeName() << "): ";
  std::string input;
  std::getline(std::cin, input);
  if (!ParseStringToScalarAnyvalue(value, input))
  {
    auto json_type = sup::dto::AnyTypeToJSONString(value.GetType());
    std::string error_message =
      "CLInterface::GetUserValue(): user provided value [" + input +
      "] could not be parsed to type [" + json_type + "]";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return false;
  }
  return true;
}

int CLInterface::GetUserChoice(const std::vector<std::string>& options,
                               const sup::dto::AnyValue& metadata)
{
  std::lock_guard<std::mutex> lk{m_mtx};
  std::string message = GetMainTextFromMetadata(metadata);
  if (message.empty())
  {
    message = "Select one of the following options:";
  }
  std::cout << message << std::endl;
  for (int i = 0; i < options.size(); ++i)
  {
    std::cout << i + 1 << ": " << options[i] << std::endl;
  }
  int input = -1;
  std::string input_str;
  std::getline(std::cin, input_str);
  std::istringstream istr(input_str);
  istr >> input;
  if (istr.fail())
  {
    std::string error_message =
      "CLInterface::GetUserChoice(): user provided value [" + input_str +
      "] could not be parsed to integer";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return -1;
  }
  // Decrement input as the interface uses 1-based indexing
  --input;
  if (input < 0 || input >= options.size())
  {
    std::string error_message =
      "CLInterface::GetUserChoice(): user provided value [" +
      std::to_string(input + 1) + "] must be in the range [1, " + std::to_string(options.size()) + "]";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return -1;
  }
  std::cout << options[input] << " selected" << std::endl;
  return input;
}

}  // namespace sequencer

}  // namespace sup

using ParseFunction = bool (*)(sup::dto::AnyValue &, const std::string &);

template <typename T>
bool ParserFunctionT(sup::dto::AnyValue &value, const std::string &str)
{
  std::istringstream istr(str);
  T val;
  istr >> val;
  if (istr.fail())
  {
    return false;
  }
  value = val;
  return true;
}

/**
 * @todo Extend possible input values: currently only 'true' or 'false'. Possible alternatives
 * are integers (zero is false), case insensitive true/false, yes/no, etc.
 */
template <>
bool ParserFunctionT<sup::dto::boolean>(sup::dto::AnyValue &value, const std::string &str)
{
  std::istringstream istr(str);
  sup::dto::boolean val;
  istr >> std::boolalpha >> val;
  if (istr.fail())
  {
    return false;
  }
  value = val;
  return true;
}

template <>
bool ParserFunctionT<std::string>(sup::dto::AnyValue &value, const std::string &str)
{
  value = str;
  return true;
}

static std::map<std::string, ParseFunction> CreateParserMap()
{
  std::map<std::string, ParseFunction> parser_map;
  parser_map["bool"] = ParserFunctionT<sup::dto::boolean>;
  parser_map["char8"] = ParserFunctionT<sup::dto::char8>;
  parser_map["int8"] = ParserFunctionT<sup::dto::int8>;
  parser_map["uint8"] = ParserFunctionT<sup::dto::uint8>;
  parser_map["int16"] = ParserFunctionT<sup::dto::int16>;
  parser_map["uint16"] = ParserFunctionT<sup::dto::uint16>;
  parser_map["int32"] = ParserFunctionT<sup::dto::int32>;
  parser_map["uint32"] = ParserFunctionT<sup::dto::uint32>;
  parser_map["uint64"] = ParserFunctionT<sup::dto::uint64>;
  parser_map["float32"] = ParserFunctionT<sup::dto::float32>;
  parser_map["float64"] = ParserFunctionT<sup::dto::float64>;
  parser_map["string"] = ParserFunctionT<std::string>;
  return parser_map;
}

static std::map<std::string, ParseFunction> &GetParserMap()
{
  static std::map<std::string, ParseFunction> parser_map = CreateParserMap();
  return parser_map;
}

static bool ParseStringToScalarAnyvalue(sup::dto::AnyValue &value, const std::string &str)
{
  std::string type_name = value.GetTypeName();

  auto &parser_map = GetParserMap();
  if (parser_map.find(type_name) == parser_map.end())
  {
    return false;
  }
  auto parse_function = parser_map[type_name];
  return parse_function(value, str);
}
