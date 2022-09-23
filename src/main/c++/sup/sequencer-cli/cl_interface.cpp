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

#include "cl_interface.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/instruction.h>

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
void CLInterface::UpdateInstructionStatusImpl(const Instruction *instruction)
{
  auto instruction_type = instruction->GetType();
  auto instruction_name = instruction->GetName();
  auto status = instruction->GetStatus();

  if (m_verbose)
  {
    std::cout << "Instruction: (" << instruction_type << ":" << instruction_name << ") : ";
    std::cout << StatusToString(status) << std::endl;
  }
}

void CLInterface::VariableUpdatedImpl(const std::string& name, const sup::dto::AnyValue& value)
{
  if (!m_verbose)
  {
    return;
  }
  std::cout << "Variable (" << name << ") updated: ";
  std::string json_rep = sup::dto::ValuesToJSONString(value);
  if (!json_rep.empty())
  {
    std::cout << json_rep << std::endl;
  }
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
    log::Warning("CLInterface::GetUserValueImpl(value, '%s') only supports scalar values..",
                description.c_str());
    return false;
  }
  std::cout << description << " (" << value.GetTypeName() << "): ";
  std::string input;
  std::getline(std::cin, input);
  bool result = ParseStringToScalarAnyvalue(value, input);
  if (!result)
  {
    log::Warning("CLInterface::GetUserValueImpl(value, '%s') could not parse '%s' into value..",
                input.c_str());
  }
  return result;
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
  if (istr.fail() || input < 0 || input >= choices.size())
  {
    log::Warning("CLInterface::GetUserChoiceImpl() - invalid choice");
    return -1;
  }
  std::cout << choices[input] << " selected" << std::endl;
  return input;
}

void CLInterface::StartSingleStepImpl()
{
  if (m_verbose)
  {
    std::cout << "Start single execution step" << std::endl;
  }
}

void CLInterface::MessageImpl(const std::string& message)
{
  std::cout << message << std::endl;
}

void CLInterface::EndSingleStepImpl()
{
  if (m_verbose)
  {
    std::cout << "End single execution step" << std::endl;
  }
}

CLInterface::CLInterface(bool verbose) : m_verbose{verbose} {}

CLInterface::~CLInterface() = default;

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
    sup::sequencer::log::Warning("ParseStringToScalarAnyvalue() - could not parse ('%s') in type ('%s)", str.c_str(),
                value.GetTypeName());
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
    sup::sequencer::log::Warning(
      "ParseStringToScalarAnyvalue() - could not parse ('%s') in type ('%s)", str.c_str(),
       value.GetTypeName());
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
