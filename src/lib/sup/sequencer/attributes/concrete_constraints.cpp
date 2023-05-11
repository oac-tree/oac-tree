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

#include <sup/sequencer/concrete_constraints.h>

#include <sup/sequencer/attribute_utils.h>

#include <sup/dto/anytype_helper.h>

#include <algorithm>

namespace sup
{
namespace sequencer
{

Exists::Exists(const std::string& attr_name)
  : m_attr_name{attr_name}
{}

Exists::~Exists() = default;

Exists* Exists::Clone() const
{
  return new Exists{m_attr_name};
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

FixedType* FixedType::Clone() const
{
  return new FixedType{m_attr_name, m_attr_type};
}

bool FixedType::Validate(const StringAttributeList& attr_map) const
{
  auto it = std::find_if(attr_map.begin(), attr_map.end(),
                         [this](const StringAttribute& str_attr) {
                           return str_attr.first == m_attr_name;
                         });
  if (it == attr_map.end())
  {
    return false;
  }
  auto val_str = it->second;
  return utils::ParseAttributeString(m_attr_type, val_str).first;
}

std::string FixedType::GetRepresentation() const
{
  return "Type of (" + m_attr_name + ") must be (" +
          sup::dto::AnyTypeToJSONString(m_attr_type) + ")";
}

Either::Either(Constraint&& left, Constraint&& right)
  : m_left{std::move(left)}
  , m_right{std::move(right)}
{}

Either::~Either() = default;

Either* Either::Clone() const
{
  return new Either{Constraint{m_left}, Constraint{m_right}};
}

bool Either::Validate(const StringAttributeList& attr_map) const
{
  return m_left.Validate(attr_map) ^ m_right.Validate(attr_map);
}

std::string Either::GetRepresentation() const
{
  return "Either (" + m_left.GetRepresentation() + ") or (" + m_right.GetRepresentation() + ")";
}

Both::Both(Constraint&& left, Constraint&& right)
  : m_left{std::move(left)}
  , m_right{std::move(right)}
{}

Both::~Both() = default;

Both* Both::Clone() const
{
  return new Both{Constraint{m_left}, Constraint{m_right}};
}

bool Both::Validate(const StringAttributeList& attr_map) const
{
  return m_left.Validate(attr_map) && m_right.Validate(attr_map);
}

std::string Both::GetRepresentation() const
{
  return "Both (" + m_left.GetRepresentation() + ") and (" + m_right.GetRepresentation() + ")";
}

}  // namespace sequencer

}  // namespace sup
