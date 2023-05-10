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

#include <algorithm>

namespace sup
{
namespace sequencer
{
AttributeHandler::AttributeHandler()
  : m_attr_validator{}
  , m_str_attributes{}
{}

AttributeHandler::~AttributeHandler() = default;

AttributeDefinition& AttributeHandler::AddAttributeDefinition(
  const std::string& attr_name, const sup::dto::AnyType& value_type)
{
  return m_attr_validator.AddAttributeDefinition(attr_name, value_type);
}

void AttributeHandler::AddConstraint(Constraint constraint)
{
  m_attr_validator.AddConstraint(std::move(constraint));
}

bool AttributeHandler::HasAttribute(const std::string& name) const
{
  auto it = std::find_if(m_str_attributes.begin(), m_str_attributes.end(),
                         [&name](const StringAttribute& str_attr){
                           return str_attr.first == name;
                         });
  return it != m_str_attributes.end();
}

bool AttributeHandler::AddAttribute(const std::string& name, const std::string& value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  m_str_attributes.emplace_back(name, value);
  return true;
}

const std::vector<AttributeDefinition>& AttributeHandler::GetAttributeDefinitions() const
{
  return m_attr_validator.GetAttributeDefinitions();
}

}  // namespace sequencer

}  // namespace sup
