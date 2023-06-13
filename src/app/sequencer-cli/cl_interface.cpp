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
 * Copyright (c) : 2010-2023 ITER Organization,
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
CLInterface::CLInterface(const sup::log::BasicLogger& logger)
  : m_logger{logger}
{}

CLInterface::~CLInterface() = default;

void CLInterface::UpdateInstructionStatusImpl(const Instruction *instruction)
{
  std::string info_message = "Instruction (" + instruction->GetType() + ":" +
    instruction->GetName() + ") : " + StatusToString(instruction->GetStatus());
  m_logger.LogMessage(log::SUP_SEQ_LOG_INFO, info_message);
}

void CLInterface::VariableUpdatedImpl(const std::string& name, const sup::dto::AnyValue& value,
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

bool CLInterface::PutValueImpl(const sup::dto::AnyValue &value, const std::string &description)
{
  std::cout << description << " (" << value.GetTypeName() << "): ";
  std::string json_rep = sup::dto::ValuesToJSONString(value);
  if (json_rep.empty())
  {
    return false;
  }
  std::cout << json_rep << std::endl;
  return true;
}

bool CLInterface::GetUserValueImpl(sup::dto::AnyValue &value, const std::string &description)
{
  if (!sup::dto::IsScalarValue(value))
  {
    auto json_type = sup::dto::AnyTypeToJSONString(value.GetType());
    std::string error_message =
      "sup::sequencer::CLInterface::GetUserValueImpl(): only scalar values are supported, "
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
      "sup::sequencer::CLInterface::GetUserValueImpl(): user provided value [" + input +
      "] could not be parsed to type [" + json_type + "]";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return false;
  }
  return true;
}

int CLInterface::GetUserChoiceImpl(const std::vector<std::string> &choices,
                                   const std::string &description)
{
  std::string message = description;
  if (message.empty())
  {
    message = "Select one of the following options:";
  }
  std::cout << message << std::endl;
  for (int i = 0; i < choices.size(); ++i)
  {
    std::cout << i << ": " << choices[i] << std::endl;
  }
  int input = -1;
  std::string input_str;
  std::getline(std::cin, input_str);
  std::istringstream istr(input_str);
  istr >> input;
  if (istr.fail())
  {
    std::string error_message =
      "sup::sequencer::CLInterface::GetUserChoiceImpl(): user provided value [" + input_str +
      "] could not be parsed to integer";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return -1;
  }
  if (input < 0 || input >= choices.size())
  {
    std::string error_message =
      "sup::sequencer::CLInterface::GetUserChoiceImpl(): user provided value [" +
      std::to_string(input) + "] must be in the range [0, " + std::to_string(choices.size()) + ")";
    m_logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return -1;
  }
  std::cout << choices[input] << " selected" << std::endl;
  return input;
}

void CLInterface::MessageImpl(const std::string& message)
{
  std::cout << message << std::endl;
}

void CLInterface::LogImpl(int severity, const std::string& message)
{
  m_logger.LogMessage(severity, message);
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
