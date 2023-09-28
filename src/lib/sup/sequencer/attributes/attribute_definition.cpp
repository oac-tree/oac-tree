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

#include <sup/sequencer/attribute_definition.h>

#include "attribute_properties.h"

namespace sup
{
namespace sequencer
{
AttributeDefinition::AttributeDefinition(const std::string& name,
                                         const sup::dto::AnyType& value_type)
  : m_name{name}
  , m_properties{new AttributeProperties(value_type)}
{}

AttributeDefinition::~AttributeDefinition() = default;

AttributeDefinition::AttributeDefinition(const AttributeDefinition& other)
  : m_name{other.m_name}
  , m_properties{new AttributeProperties(*other.m_properties)}
{}

AttributeDefinition::AttributeDefinition(AttributeDefinition&& other) noexcept = default;

AttributeDefinition& AttributeDefinition::operator=(const AttributeDefinition& other) &
{
  m_name = other.m_name;
  *m_properties = *other.m_properties;
  return *this;
}

AttributeDefinition& AttributeDefinition::operator=(AttributeDefinition&& other) & noexcept = default;

std::string AttributeDefinition::GetName() const
{
  return m_name;
}

sup::dto::AnyType AttributeDefinition::GetType() const
{
  return m_properties->GetType();
}

bool AttributeDefinition::IsMandatory() const
{
  return m_properties->IsMandatory();
}

AttributeDefinition& AttributeDefinition::SetMandatory(bool mandatory)
{
  m_properties->SetMandatory(mandatory);
  return *this;
}

}  // namespace sequencer

}  // namespace sup
