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
#include <sup/sequencer/constants.h>
#include <sup/sequencer/workspace.h>

#include <sup/sequencer/attributes/attribute_validator.h>
#include <sup/sequencer/instructions/instruction_helper.h>

#include <algorithm>
#include <sstream>

namespace sup
{
namespace sequencer
{
struct AttributeHandler::AttributeHandlerImpl
{
  AttributeValidator attr_validator;
  std::vector<std::string> failed_constraints;
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

bool AttributeHandler::ValidateAttributes()
{
  m_impl->failed_constraints = m_impl->attr_validator.ValidateAttributes(m_str_attributes);
  return m_impl->failed_constraints.empty();
}

void AttributeHandler::ClearFailedConstraints()
{
  m_impl->failed_constraints.clear();
}

std::vector<std::string> AttributeHandler::GetFailedConstraints() const
{
  return m_impl->failed_constraints;
}

sup::dto::AnyValue AttributeHandler::GetValue(const std::string& attr_name) const
{
  const auto str_attr = GetStringAttribute(attr_name);
  return TryCreateAnyValue(str_attr);
}

sup::dto::AnyValue AttributeHandler::GetValue(const std::string& attr_name,
                                              const Workspace& ws) const
{
  const auto str_attr = GetStringAttribute(attr_name);
  const auto str_attr_val = str_attr.second;
  if (InstructionHelper::AttributeStartsWith(str_attr_val,
                                             DefaultSettings::WORKSPACE_ATTRIBUTE_CHAR))
  {
    sup::dto::AnyValue result;
    const auto var_name = str_attr_val.substr(1u);
    if (!ws.GetValue(var_name, result))
    {
      std::string message = "AttributeHandler::GetValue(): could not fetch variable value [" +
                            var_name + "] from workspace";
      throw RuntimeException(message);
    }
    return result;
  }
  return TryCreateAnyValue(str_attr);
}

StringAttribute AttributeHandler::GetStringAttribute(const std::string& attr_name) const
{
  auto it = FindStringAttribute(m_str_attributes, attr_name);
  if (it == m_str_attributes.end())
  {
    std::string message = "AttributeHandler::GetStringAttribute(): no attribute with name [" +
                          attr_name + "]";
    throw RuntimeException(message);
  }
  return *it;
}

sup::dto::AnyValue AttributeHandler::TryCreateAnyValue(const StringAttribute& str_attr) const
{
  auto result = m_impl->attr_validator.TryCreateAnyValue(str_attr);
  if (!result.second.empty())
  {
    std::string message = "AttributeHandler::TryCreateAnyValue(): failed to create value of name ["
                          + str_attr.first + "] because of constraint:\n" + result.second;
    throw RuntimeException(message);
  }
  return result.first;
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

std::string FormatFailedConstraints(const std::vector<std::string>& failed_constraints,
                                    const std::string& prefix)
{
  std::stringstream oss;
  for (const auto& failed_constraint : failed_constraints)
  {
    oss << prefix << failed_constraint;
  }
  return oss.str();
}
}  // namespace sequencer

}  // namespace sup
