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

#include <sup/sequencer/attribute_map.h>

#include <sup/sequencer/constants.h>

#include <algorithm>
#include <cctype>
#include <type_traits>

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
                           std::pair<std::string, std::string>>::value,
              "Exposed iterator must point to std::pair<std::string, std::string>");
static_assert(std::is_same<AttributeMap::map_type::const_iterator::value_type,
                           std::pair<std::string, std::string>>::value,
              "Exposed const_iterator must point to std::pair<const std::string, std::string>");

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

AttributeMap::AttributeMap(const AttributeMap::map_type& attr_list)
  : m_attributes{}
{
  for (auto& attr : attr_list)
  {
    AddAttribute(attr.first, attr.second);
  }
}

AttributeMap::AttributeMap(const AttributeMap&) = default;
AttributeMap::AttributeMap(AttributeMap&&) = default;

AttributeMap& AttributeMap::operator=(const AttributeMap&) = default;
AttributeMap& AttributeMap::operator=(AttributeMap&&) = default;

bool AttributeMap::operator==(const AttributeMap& other) const
{
  return m_attributes == other.m_attributes;
}
bool AttributeMap::operator!=(const AttributeMap& other) const
{
  return !this->operator==(other);
}

size_t AttributeMap::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

bool AttributeMap::HasAttribute(const std::string& name) const
{
  auto it = Find(name);
  return it != m_attributes.end();
}

std::string AttributeMap::GetAttribute(const std::string& name) const
{
  auto it = Find(name);
  if (it == m_attributes.end())
  {
    return {};
  }
  return it->second;
}

std::vector<std::string> AttributeMap::GetAttributeNames() const
{
  std::vector<std::string> result;
  std::transform(m_attributes.begin(), m_attributes.end(), std::back_inserter(result),
                 [](const Attribute& attr)
                 {
                  return attr.first;
                 });
  return result;
}

bool AttributeMap::AddAttribute(const std::string& name, const std::string& value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  m_attributes.emplace_back(name, value);
  return true;
}

void AttributeMap::SetAttribute(const std::string& name, const std::string& value)
{
  auto it = Find(name);
  if (it == m_attributes.end())
  {
    m_attributes.emplace_back(name, value);
  }
  else
  {
    it->second = value;
  }
}

void AttributeMap::Clear()
{
  m_attributes.clear();
}

bool AttributeMap::Remove(const std::string& name)
{
  auto it = Find(name);
  if (it != m_attributes.end())
  {
    m_attributes.erase(it);
    return true;
  }
  return false;
}

bool AttributeMap::InitialiseVariableAttributes(const AttributeMap& source)
{
  bool result = true;
  for (auto& attr : m_attributes)
  {
    auto attr_name = attr.first;
    auto attr_value = attr.second;
    if (StartsWith(attr_value, DefaultSettings::VAR_ATTRIBUTE_CHAR))
    {
      auto var_name = attr_value.substr(1);
      if (!source.HasAttribute(var_name))
      {
        result = false;
        continue;
      }
      auto var_value = source.GetAttribute(var_name);
      attr.second = var_value;
    }
  }
  return result;
}

AttributeMap::map_type::iterator AttributeMap::Find(const std::string& name)
{
  return std::find_if(m_attributes.begin(), m_attributes.end(),
                      [name](const Attribute& attr)
                      {
                        return attr.first == name;
                      });
}

AttributeMap::map_type::const_iterator AttributeMap::Find(const std::string& name) const
{
  return std::find_if(m_attributes.begin(), m_attributes.end(),
                      [name](const Attribute& attr)
                      {
                        return attr.first == name;
                      });
}

}  // namespace sequencer

}  // namespace sup

namespace
{
bool StartsWith(const std::string& str, char c)
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
