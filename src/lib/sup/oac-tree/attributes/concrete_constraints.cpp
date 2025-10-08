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

#include <sup/oac-tree/concrete_constraints.h>

#include <sup/oac-tree/attribute_utils.h>
#include <sup/oac-tree/constraint.h>

#include <sup/dto/anytype_helper.h>

#include <algorithm>
#include <memory>

namespace sup
{
namespace oac_tree
{

Exists::Exists(const std::string& attr_name)
  : m_attr_name{attr_name}
{}

Exists::~Exists() = default;

std::unique_ptr<IConstraint> Exists::Clone() const
{
  return std::make_unique<Exists>(m_attr_name);
}

bool Exists::Validate(const StringAttributeList& attr_map) const
{
  auto it = std::find_if(attr_map.begin(), attr_map.end(),
                         [this](const StringAttribute& str_attr) {
                           return str_attr.first == m_attr_name;
                         });
  return it != attr_map.end();
}

std::string Exists::GetRepresentation() const
{
  return "Exists(" + m_attr_name + ")";
}

FixedType::FixedType(const std::string& attr_name, const sup::dto::AnyType& attr_type)
  : m_attr_name{attr_name}
  , m_attr_type{attr_type}
{}

FixedType::~FixedType() = default;

std::unique_ptr<IConstraint> FixedType::Clone() const
{
  return std::make_unique<FixedType>(m_attr_name, m_attr_type);
}

bool FixedType::Validate(const StringAttributeList& attr_map) const
{
  auto result = false;
  auto it = std::find_if(attr_map.begin(), attr_map.end(),
                         [this](const StringAttribute& str_attr) {
                           return str_attr.first == m_attr_name;
                         });
  if (it == attr_map.end())
  {
    return result;
  }
  auto val_str = it->second;
  if (utils::ParseAttributeString(m_attr_type, val_str).first != false)
  {
    result = true;
  }
  return result;
}

std::string FixedType::GetRepresentation() const
{
  return "Type of (" + m_attr_name + ") must be (" +
          sup::dto::AnyTypeToJSONString(m_attr_type) + ")";
}

Xor::Xor(Constraint&& left, Constraint&& right)
  : m_left{std::move(left)}
  , m_right{std::move(right)}
{}

Xor::~Xor() = default;

std::unique_ptr<IConstraint> Xor::Clone() const
{
  return std::make_unique<Xor>(Constraint{m_left}, Constraint{m_right});
}

bool Xor::Validate(const StringAttributeList& attr_map) const
{
  return (m_left.Validate(attr_map) ^ m_right.Validate(attr_map));
}

std::string Xor::GetRepresentation() const
{
  return "(" + m_left.GetRepresentation() + ") XOR (" + m_right.GetRepresentation() + ")";
}

Or::Or(Constraint&& left, Constraint&& right)
  : m_left{std::move(left)}
  , m_right{std::move(right)}
{}

Or::~Or() = default;

std::unique_ptr<IConstraint> Or::Clone() const
{
  return std::make_unique<Or>(Constraint{m_left}, Constraint{m_right});
}

bool Or::Validate(const StringAttributeList& attr_map) const
{
  return (m_left.Validate(attr_map) || m_right.Validate(attr_map));
}

std::string Or::GetRepresentation() const
{
  return "(" + m_left.GetRepresentation() + ") OR (" + m_right.GetRepresentation() + ")";
}

And::And(Constraint&& left, Constraint&& right)
  : m_left{std::move(left)}
  , m_right{std::move(right)}
{}

And::~And() = default;

std::unique_ptr<IConstraint> And::Clone() const
{
  return std::make_unique<And>(Constraint{m_left}, Constraint{m_right});
}

bool And::Validate(const StringAttributeList& attr_map) const
{
  return (m_left.Validate(attr_map) && m_right.Validate(attr_map));
}

std::string And::GetRepresentation() const
{
  return "(" + m_left.GetRepresentation() + ") AND (" + m_right.GetRepresentation() + ")";
}

Not::Not(Constraint&& child)
  : m_child{std::move(child)}
{}

Not::~Not() = default;

std::unique_ptr<IConstraint> Not::Clone() const
{
  return std::make_unique<Not>(Constraint{m_child});
}

bool Not::Validate(const StringAttributeList& attr_map) const
{
  return !m_child.Validate(attr_map);
}

std::string Not::GetRepresentation() const
{
  return "Not (" + m_child.GetRepresentation() + ")";
}

}  // namespace oac_tree

}  // namespace sup
