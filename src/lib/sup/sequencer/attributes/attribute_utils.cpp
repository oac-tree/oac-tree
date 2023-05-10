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

#include <sup/sequencer/attribute_utils.h>

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
std::pair<bool, sup::dto::AnyValue> BooleanParserFunction(const sup::dto::AnyType& anytype,
                                                         const std::string& repr);
std::pair<bool, sup::dto::AnyValue> JSONParserFunction(const sup::dto::AnyType& anytype,
                                                       const std::string& repr);
std::pair<bool, sup::dto::AnyValue> StringParserFunction(const sup::dto::AnyType& anytype,
                                                         const std::string& repr);
bool IsTrueStringRepresentation(std::string repr);
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

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

namespace
{
const std::map<sup::dto::TypeCode, AttributeStringParser>& GetAttributeStringParserMap()
{
  static const std::map<sup::dto::TypeCode, AttributeStringParser> parser_map = {
    { sup::dto::TypeCode::Bool, BooleanParserFunction },
    { sup::dto::TypeCode::Char8, JSONParserFunction },
    { sup::dto::TypeCode::Int8, JSONParserFunction },
    { sup::dto::TypeCode::UInt8, JSONParserFunction },
    { sup::dto::TypeCode::Int16, JSONParserFunction },
    { sup::dto::TypeCode::UInt16, JSONParserFunction },
    { sup::dto::TypeCode::Int32, JSONParserFunction },
    { sup::dto::TypeCode::UInt32, JSONParserFunction },
    { sup::dto::TypeCode::Int64, JSONParserFunction },
    { sup::dto::TypeCode::UInt64, JSONParserFunction },
    { sup::dto::TypeCode::Float32, JSONParserFunction },
    { sup::dto::TypeCode::Float64, JSONParserFunction },
    { sup::dto::TypeCode::String, StringParserFunction }
  };
  return parser_map;
}

std::pair<bool, sup::dto::AnyValue> BooleanParserFunction(const sup::dto::AnyType& anytype,
                                                         const std::string& repr)
{
  if (IsTrueStringRepresentation(repr))
  {
    return { true, { sup::dto::BooleanType, true }};
  }
  return { true, { sup::dto::BooleanType, false }};
}

std::pair<bool, sup::dto::AnyValue> JSONParserFunction(const sup::dto::AnyType& anytype,
                                                       const std::string& repr)
{
  sup::dto::JSONAnyValueParser parser;
  if (!parser.TypedParseString(anytype, repr))
  {
    return { false, {} };
  }
  return { true, parser.MoveAnyValue() };
}

std::pair<bool, sup::dto::AnyValue> StringParserFunction(const sup::dto::AnyType& anytype,
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

}  // unnamed namespace

