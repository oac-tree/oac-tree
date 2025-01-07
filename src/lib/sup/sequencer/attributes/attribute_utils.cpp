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

#include <sup/sequencer/attribute_utils.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_value_parser.h>

#include <algorithm>
#include <cctype>
#include <map>
#include <vector>

namespace
{
const std::vector<std::string> kTrueStringList = {"true", "yes", "on"};
using AttributeStringParser = std::pair<bool, sup::dto::AnyValue>(*)(const sup::dto::AnyType&,
                                                                     const std::string&);
const std::map<sup::dto::TypeCode, AttributeStringParser>& GetAttributeStringParserMap();
std::pair<bool, sup::dto::AnyValue> BooleanStringParser(const sup::dto::AnyType& anytype,
                                                        const std::string& repr);
std::pair<bool, sup::dto::AnyValue> JSONStringParser(const sup::dto::AnyType& anytype,
                                                     const std::string& repr);
std::pair<bool, sup::dto::AnyValue> StringStringParser(const sup::dto::AnyType& anytype,
                                                       const std::string& repr);
bool IsTrueStringRepresentation(std::string repr);
using AttributeValueParser = std::string(*)(const sup::dto::AnyValue&);
const std::map<sup::dto::TypeCode, AttributeValueParser>& GetAttributeValueParserMap();
std::string BooleanValueParser(const sup::dto::AnyValue& value);
std::string JSONValueParser(const sup::dto::AnyValue& value);
std::string StringValueParser(const sup::dto::AnyValue& value);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{
namespace utils
{
std::pair<bool, sup::dto::AnyValue> ParseAttributeString(const sup::dto::AnyType& anytype,
                                                         const std::string& repr)
{
  const auto& parser_map = GetAttributeStringParserMap();
  auto it = parser_map.find(anytype.GetTypeCode());
  if (it == parser_map.end())
  {
    return { false, {} };
  }
  return it->second(anytype, repr);
}

std::pair<bool, std::string> CreateAttributeString(const sup::dto::AnyValue& value)
{
  const auto& parser_map = GetAttributeValueParserMap();
  auto it = parser_map.find(value.GetTypeCode());
  if (it == parser_map.end())
  {
    return { false, {} };
  }
  return { true, it->second(value) };
}

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

namespace
{
const std::map<sup::dto::TypeCode, AttributeStringParser>& GetAttributeStringParserMap()
{
  static const std::map<sup::dto::TypeCode, AttributeStringParser> parser_map = {
    { sup::dto::TypeCode::Bool, BooleanStringParser },
    { sup::dto::TypeCode::Char8, JSONStringParser },
    { sup::dto::TypeCode::Int8, JSONStringParser },
    { sup::dto::TypeCode::UInt8, JSONStringParser },
    { sup::dto::TypeCode::Int16, JSONStringParser },
    { sup::dto::TypeCode::UInt16, JSONStringParser },
    { sup::dto::TypeCode::Int32, JSONStringParser },
    { sup::dto::TypeCode::UInt32, JSONStringParser },
    { sup::dto::TypeCode::Int64, JSONStringParser },
    { sup::dto::TypeCode::UInt64, JSONStringParser },
    { sup::dto::TypeCode::Float32, JSONStringParser },
    { sup::dto::TypeCode::Float64, JSONStringParser },
    { sup::dto::TypeCode::String, StringStringParser }
  };
  return parser_map;
}

std::pair<bool, sup::dto::AnyValue> BooleanStringParser(const sup::dto::AnyType& anytype,
                                                        const std::string& repr)
{
  if (IsTrueStringRepresentation(repr))
  {
    return { true, { sup::dto::BooleanType, true }};
  }
  return { true, { sup::dto::BooleanType, false }};
}

std::pair<bool, sup::dto::AnyValue> JSONStringParser(const sup::dto::AnyType& anytype,
                                                     const std::string& repr)
{
  sup::dto::JSONAnyValueParser parser;
  if (!parser.TypedParseString(anytype, repr))
  {
    return { false, {} };
  }
  return { true, parser.MoveAnyValue() };
}

std::pair<bool, sup::dto::AnyValue> StringStringParser(const sup::dto::AnyType& anytype,
                                                       const std::string& repr)
{
  (void)anytype;
  return { true, { sup::dto::StringType, repr }};
}

bool IsTrueStringRepresentation(std::string repr)
{
  std::transform(repr.begin(), repr.end(), repr.begin(),
                 [](unsigned char c){ return std::tolower(c); });
  auto it = std::find(kTrueStringList.begin(), kTrueStringList.end(), repr);
  return it != kTrueStringList.end();
}

const std::map<sup::dto::TypeCode, AttributeValueParser>& GetAttributeValueParserMap()
{
  static const std::map<sup::dto::TypeCode, AttributeValueParser> parser_map = {
    { sup::dto::TypeCode::Bool, BooleanValueParser },
    { sup::dto::TypeCode::Char8, JSONValueParser },
    { sup::dto::TypeCode::Int8, JSONValueParser },
    { sup::dto::TypeCode::UInt8, JSONValueParser },
    { sup::dto::TypeCode::Int16, JSONValueParser },
    { sup::dto::TypeCode::UInt16, JSONValueParser },
    { sup::dto::TypeCode::Int32, JSONValueParser },
    { sup::dto::TypeCode::UInt32, JSONValueParser },
    { sup::dto::TypeCode::Int64, JSONValueParser },
    { sup::dto::TypeCode::UInt64, JSONValueParser },
    { sup::dto::TypeCode::Float32, JSONValueParser },
    { sup::dto::TypeCode::Float64, JSONValueParser },
    { sup::dto::TypeCode::String, StringValueParser }
  };
  return parser_map;
}

std::string BooleanValueParser(const sup::dto::AnyValue& value)
{
  if (value.As<sup::dto::boolean>())
  {
    return "true";
  }
  return "false";
}

std::string JSONValueParser(const sup::dto::AnyValue& value)
{
  return sup::dto::ValuesToJSONString(value);
}

std::string StringValueParser(const sup::dto::AnyValue& value)
{
  return value.As<std::string>();
}

}  // unnamed namespace

