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

#include <sup/sequencer/attribute_handler.h>

#include "attribute_validator.h"
#include "value_map_info.h"

#include <algorithm>

namespace sup
{
namespace sequencer
{
struct AttributeHandler::AttributeHandlerImpl
{
  AttributeValidator attr_validator;
  ValueMapInfo value_map_info;
};

AttributeHandler::AttributeHandler()
  : m_impl{new AttributeHandlerImpl{}}
  , m_str_attributes{}
{}

AttributeHandler::~AttributeHandler() = default;

AttributeDefinition& AttributeHandler::AddAttributeDefinition(
  const std::string& attr_name, const sup::dto::AnyType& value_type)
{
  return m_impl->attr_validator.AddAttributeDefinition(attr_name, value_type);
}

const std::vector<AttributeDefinition>& AttributeHandler::GetAttributeDefinitions() const
{
  return m_impl->attr_validator.GetAttributeDefinitions();
}

void AttributeHandler::AddConstraint(Constraint constraint)
{
  m_impl->attr_validator.AddConstraint(std::move(constraint));
}

bool AttributeHandler::HasStringAttribute(const std::string& name) const
{
  auto it = FindStringAttribute(m_str_attributes, name);
  return it != m_str_attributes.end();
}

bool AttributeHandler::AddStringAttribute(const std::string& name, const std::string& value)
{
  if (HasStringAttribute(name))
  {
    return false;
  }
  m_str_attributes.emplace_back(name, value);
  return true;
}

void AttributeHandler::SetStringAttribute(const std::string& name, const std::string& value)
{
  auto it = FindStringAttribute(m_str_attributes, name);
  if (it != m_str_attributes.end())
  {
    it->second = value;
  }
  else
  {
    m_str_attributes.emplace_back(name, value);
  }
}

const StringAttributeList& AttributeHandler::GetStringAttributes() const
{
  return m_str_attributes;
}

bool AttributeHandler::InitValueMap()
{
  m_impl->value_map_info = m_impl->attr_validator.CreateValueMap(m_str_attributes);
  return m_impl->value_map_info.failed_constraints.empty();
}

void AttributeHandler::ClearValueMap()
{
  m_impl->value_map_info.failed_constraints.clear();
  m_impl->value_map_info.value_map.clear();
}

std::vector<std::string> AttributeHandler::GetFailedConstraints() const
{
  return m_impl->value_map_info.failed_constraints;
}

sup::dto::AnyValue AttributeHandler::GetValue(const std::string& name) const
{
  auto it = m_impl->value_map_info.value_map.find(name);
  if (it == m_impl->value_map_info.value_map.end())
  {
    return {};
  }
  return it->second;
}

std::string GetStringAttributeValue(const StringAttributeList& str_attributes,
                                    const std::string& attr_name)
{
  auto it = FindStringAttribute(str_attributes, attr_name);
  if (it == str_attributes.end())
  {
    return {};
  }
  return it->second;
}

class StringAttributeNameMatcher
{
public:
  StringAttributeNameMatcher(std::string name) : m_name{std::move(name)} {}
  ~StringAttributeNameMatcher() = default;
  bool operator()(const StringAttribute& str_attr) { return str_attr.first == m_name; }
private:
  const std::string m_name;
};

StringAttributeList::iterator FindStringAttribute(StringAttributeList& str_attributes,
                                                  const std::string& attr_name)
{
  auto predicate = StringAttributeNameMatcher(attr_name);
  return std::find_if(str_attributes.begin(), str_attributes.end(), predicate);
}

StringAttributeList::const_iterator FindStringAttribute(const StringAttributeList& str_attributes,
                                                        const std::string& attr_name)
{
  auto predicate = StringAttributeNameMatcher(attr_name);
  return std::find_if(str_attributes.begin(), str_attributes.end(), predicate);
}

}  // namespace sequencer

}  // namespace sup
