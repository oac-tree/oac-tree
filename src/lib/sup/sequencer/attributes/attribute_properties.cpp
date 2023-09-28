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

#include "attribute_properties.h"

namespace sup
{
namespace sequencer
{

AttributeProperties::AttributeProperties(const sup::dto::AnyType& value_type)
  : m_value_type{value_type}
  , m_is_mandatory{false}
{}

AttributeProperties::~AttributeProperties() = default;

sup::dto::AnyType AttributeProperties::GetType() const
{
  return m_value_type;
}

bool AttributeProperties::IsMandatory() const
{
  return m_is_mandatory;
}

void AttributeProperties::SetMandatory(bool mandatory)
{
  m_is_mandatory = mandatory;
}

}  // namespace sequencer

}  // namespace sup
