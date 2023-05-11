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

#include <sup/sequencer/attribute_validator.h>

#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/value_map_info.h>

#include <algorithm>

namespace
{
using sup::sequencer::AttributeDefinition;
using sup::sequencer::StringAttribute;
using sup::sequencer::StringAttributeList;
using sup::sequencer::ValueMapInfo;
using sup::sequencer::Constraint;
std::vector<AttributeDefinition>::const_iterator FindAttributeDefinition(
  const std::vector<AttributeDefinition>& attribute_definitions,
  const std::string& attr_name);

void CreateValueEntryFromStringAttribute(const StringAttribute& str_attr,
                                         std::vector<AttributeDefinition>& attr_defs,
                                         ValueMapInfo& value_map_info);

void HandleRemainingMandatoryAttributes(const std::vector<AttributeDefinition>& attr_defs,
                                        ValueMapInfo& value_map_info);

void ValidateCustomConstraints(const StringAttributeList& str_attributes,
                               const std::vector<Constraint>& constraints,
                               ValueMapInfo& value_map_info);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{
AttributeValidator::AttributeValidator()
  : m_attribute_definitions{}
  , m_custom_constraints{}
{}

AttributeValidator::~AttributeValidator() = default;

AttributeDefinition& AttributeValidator::AddAttributeDefinition(
  const std::string& attr_name, const sup::dto::AnyType& value_type)
{
  if (HasAttributeDefinition(attr_name))
  {
    std::string error_message = "AttributeValidator::AddAttributeDefinition(): trying to add already "
      "existing attribute with name (" + attr_name + ")";
    throw InvalidOperationException(error_message);
  }
  m_attribute_definitions.emplace_back(attr_name, value_type);
  return m_attribute_definitions.back();
}

void AttributeValidator::AddConstraint(Constraint constraint)
{
  m_custom_constraints.push_back(std::move(constraint));
}

const std::vector<AttributeDefinition>& AttributeValidator::GetAttributeDefinitions() const
{
  return m_attribute_definitions;
}

ValueMapInfo AttributeValidator::CreateValueMap(const StringAttributeList& str_attributes) const
{
  ValueMapInfo result;
  auto attr_defs = m_attribute_definitions;
  for (const auto& str_attr : str_attributes)
  {
    CreateValueEntryFromStringAttribute(str_attr, attr_defs, result);
  }
  HandleRemainingMandatoryAttributes(attr_defs, result);
  ValidateCustomConstraints(str_attributes, m_custom_constraints, result);
  // Clear value map when not all constraints were successfully validated:
  if (result.failed_constraints.size() > 0)
  {
    result.value_map.clear();
  }
  return result;
}

bool AttributeValidator::HasAttributeDefinition(const std::string& attr_name) const
{
  auto it = FindAttributeDefinition(m_attribute_definitions, attr_name);
  return it != m_attribute_definitions.end();
}

}  // namespace sequencer

}  // namespace sup

namespace
{
std::vector<sup::sequencer::AttributeDefinition>::const_iterator FindAttributeDefinition(
  const std::vector<sup::sequencer::AttributeDefinition>& attribute_definitions,
  const std::string& attr_name)
{
  auto predicate = [&attr_name](const sup::sequencer::AttributeDefinition& attr_def)
                   {
                     return attr_def.GetName() == attr_name;
                   };
  return std::find_if(attribute_definitions.begin(), attribute_definitions.end(), predicate);
}

void CreateValueEntryFromStringAttribute(const StringAttribute& str_attr,
                                         std::vector<AttributeDefinition>& attr_defs,
                                         ValueMapInfo& value_map_info)
{
  using sup::sequencer::MakeConstraint;
  auto it = FindAttributeDefinition(attr_defs, str_attr.first);
  if (it == attr_defs.end())
  {
    value_map_info.value_map.emplace(str_attr.first, sup::dto::AnyValue(str_attr.second));
    return;
  }
  auto attr_type = it->GetType();
  auto parsed = sup::sequencer::utils::ParseAttributeString(attr_type, str_attr.second);
  if (parsed.first)
  {
    value_map_info.value_map.emplace(str_attr.first, parsed.second);
  }
  else
  {
    value_map_info.failed_constraints.emplace_back(
        MakeConstraint<sup::sequencer::FixedType>(str_attr.first, attr_type).GetRepresentation());
  }
  attr_defs.erase(it);
}

void HandleRemainingMandatoryAttributes(const std::vector<AttributeDefinition>& attr_defs,
                                        ValueMapInfo& value_map_info)
{
  using sup::sequencer::MakeConstraint;
  for (const auto& attr_def : attr_defs)
  {
    if (attr_def.IsMandatory())
    {
      value_map_info.failed_constraints.emplace_back(
        MakeConstraint<sup::sequencer::Exists>(attr_def.GetName()).GetRepresentation());
    }
  }
}

void ValidateCustomConstraints(const StringAttributeList& str_attributes,
                               const std::vector<Constraint>& constraints,
                               ValueMapInfo& value_map_info)
{
  for (const auto& constraint : constraints)
  {
    if (!constraint.Validate(str_attributes))
    {
      value_map_info.failed_constraints.push_back(constraint.GetRepresentation());
    }
  }
}

}  // unnamed namespace
