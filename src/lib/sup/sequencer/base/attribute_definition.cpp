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

namespace sup
{
namespace sequencer
{
AttributeDefinition::AttributeDefinition(const std::string& name)
  : m_name{name}
  , m_value_type{}
  , m_is_mandatory{false}
{}

AttributeDefinition::~AttributeDefinition() = default;

std::string AttributeDefinition::GetName() const
{
  return m_name;
}

sup::dto::AnyType AttributeDefinition::GetType() const
{
  return m_value_type;
}

bool AttributeDefinition::IsMandatory() const
{
  return m_is_mandatory;
}

AttributeDefinition& AttributeDefinition::SetType(const sup::dto::AnyType& value_type)
{
  m_value_type = value_type;
  return *this;
}

AttributeDefinition& AttributeDefinition::SetMandatory(bool mandatory)
{
  m_is_mandatory = mandatory;
  return *this;
}

}  // namespace sequencer

}  // namespace sup
