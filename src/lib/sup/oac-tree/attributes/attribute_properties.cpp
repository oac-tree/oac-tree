/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "attribute_properties.h"

namespace sup
{
namespace oac_tree
{

AttributeProperties::AttributeProperties(const sup::dto::AnyType& value_type)
  : m_value_type{value_type}
  , m_is_mandatory{false}
  , m_category{AttributeCategory::kLiteral}
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

AttributeCategory AttributeProperties::GetCategory() const
{
  return m_category;
}

void AttributeProperties::SetMandatory(bool mandatory)
{
  m_is_mandatory = mandatory;
}

void AttributeProperties::SetCategory(AttributeCategory cat)
{
  m_category = cat;
}

}  // namespace oac_tree

}  // namespace sup
