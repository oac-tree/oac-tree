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

#include "AttributeMap.h"
#include "Constants.h"

#include <algorithm>
#include <cctype>
#include <type_traits>
#include <common/log-api.h>

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace
{
const std::vector<std::string> TRUE_LIST = {"true", "yes", "on"};
bool StartsWith(const std::string &str, char c);
std::string StringToLower(std::string s);
}  // namespace

namespace sup
{
namespace sequencer
{
// Static assertion
static_assert(std::is_same<AttributeMap::map_type::iterator::value_type,
                           std::pair<const std::string, std::string>>::value,
              "Exposed iterator must point to std::pair<const std::string, "
              "std::string>");
static_assert(std::is_same<AttributeMap::map_type::const_iterator::value_type,
                           std::pair<const std::string, std::string>>::value,
              "Exposed const_iterator must point to std::pair<const std::string, "
              "std::string>");

namespace attributes
{
bool AttributeAsBool(const std::string& value)
{
  auto it = std::find(TRUE_LIST.begin(), TRUE_LIST.end(), StringToLower(value));
  return it != TRUE_LIST.end();
}
}

AttributeMap::AttributeMap() = default;

AttributeMap::~AttributeMap() = default;

AttributeMap::AttributeMap(const AttributeMap &) = default;
AttributeMap::AttributeMap(AttributeMap &&) = default;

AttributeMap &AttributeMap::operator=(const AttributeMap &) = default;
AttributeMap &AttributeMap::operator=(AttributeMap &&) = default;

bool AttributeMap::operator==(const AttributeMap &other) const
{
  return _attributes == other._attributes;
}
bool AttributeMap::operator!=(const AttributeMap &other) const
{
  return !this->operator==(other);
}

size_t AttributeMap::GetNumberOfAttributes() const
{
  return _attributes.size();
}

bool AttributeMap::HasAttribute(const std::string &name) const
{
  return _attributes.find(name) != _attributes.end();
}

std::string AttributeMap::GetAttribute(const std::string &name) const
{
  if (!HasAttribute(name))
  {
    return {};
  }
  return _attributes.at(name);
}

std::vector<std::string> AttributeMap::GetAttributeNames() const
{
  std::vector<std::string> result;
  for (auto &pair : _attributes)
  {
    result.push_back(pair.first);
  }
  return result;
}

bool AttributeMap::AddAttribute(const std::string &name, const std::string &value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  _attributes[name] = value;
  return true;
}

void AttributeMap::SetAttribute(const std::string &name, const std::string &value)
{
  _attributes[name] = value;
}

void AttributeMap::Clear()
{
  _attributes.clear();
}

bool AttributeMap::Remove(const std::string &name)
{
  return _attributes.erase(name) > 0;
}

bool AttributeMap::InitialiseVariableAttributes(const AttributeMap &source)
{
  bool result = true;
  for (const auto &attr_name : GetAttributeNames())
  {
    auto attr_value = GetAttribute(attr_name);
    if (StartsWith(attr_value, DefaultSettings::VAR_ATTRIBUTE_CHAR))
    {
      log_debug(
          "AttributeMap::InitialiseVariableAttributes() - attr_name('%s'), "
          "attr_value('%s')",
          attr_name.c_str(), attr_value.c_str());
      auto var_name = attr_value.substr(1);
      if (!source.HasAttribute(var_name))
      {
        result = false;
        continue;
      }
      auto var_value = source.GetAttribute(var_name);
      log_debug(
          "AttributeMap::InitialiseVariableAttributes() - set attr_name('%s') "
          "to '%s'",
          attr_name.c_str(), var_value.c_str());
      _attributes[attr_name] = var_value;
    }
  }
  return result;
}

}  // namespace sequencer

}  // namespace sup

namespace
{
bool StartsWith(const std::string &str, char c)
{
  if (str.empty())
  {
    return false;
  }
  return str[0] == c;
}

std::string StringToLower(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c){ return std::tolower(c); });
  return s;
}

}  // namespace

#undef LOG_ALTERN_SRC
