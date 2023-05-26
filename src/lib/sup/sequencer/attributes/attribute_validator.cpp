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

#include "attribute_validator.h"

#include <sup/sequencer/attribute_handler.h>
#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/exceptions.h>

#include <algorithm>

namespace
{
using sup::sequencer::AttributeDefinition;
using sup::sequencer::StringAttribute;
using sup::sequencer::StringAttributeList;
using sup::sequencer::Constraint;

std::vector<std::string> CheckMandatoryConstraints(
  const std::vector<AttributeDefinition>& attr_defs, const StringAttributeList& str_attributes);

std::vector<AttributeDefinition>::const_iterator FindAttributeDefinition(
  const std::vector<AttributeDefinition>& attribute_definitions,
  const std::string& attr_name);
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

std::vector<std::string> AttributeValidator::ValidateAttributes(
  const StringAttributeList& str_attributes) const
{
  auto failed_constraints = CheckMandatoryConstraints(m_attribute_definitions, str_attributes);
  for (const auto& str_attr : str_attributes)
  {
    auto result = TryCreateAnyValue(str_attr);
    if (!result.second.empty())
    {
      failed_constraints.push_back(result.second);
    }
  }
  for (const auto& constraint : m_custom_constraints)
  {
    if (!constraint.Validate(str_attributes))
    {
      failed_constraints.push_back(constraint.GetRepresentation());
    }
  }
  return failed_constraints;
}

std::pair<sup::dto::AnyValue, std::string> AttributeValidator::TryCreateAnyValue(
  const StringAttribute& str_attr) const
{
  auto it = FindAttributeDefinition(m_attribute_definitions, str_attr.first);
  if (it == m_attribute_definitions.end())
  {
    return { sup::dto::AnyValue(str_attr.second), "" };
  }
  auto attr_type = it->GetType();
  auto parsed = sup::sequencer::utils::ParseAttributeString(attr_type, str_attr.second);
  if (parsed.first)
  {
    return { parsed.second, "" };
  }
  auto failed_constraint =
    MakeConstraint<sup::sequencer::FixedType>(str_attr.first, attr_type).GetRepresentation();
  return { {}, failed_constraint };
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

std::vector<std::string> CheckMandatoryConstraints(
  const std::vector<AttributeDefinition>& attr_defs, const StringAttributeList& str_attributes)
{
  using sup::sequencer::MakeConstraint;
  std::vector<std::string> failed_constraints;
  for (const auto& attr_def : attr_defs)
  {
    if (attr_def.IsMandatory())
    {
      auto it = sup::sequencer::FindStringAttribute(str_attributes, attr_def.GetName());
      if (it == str_attributes.end())
      {
      failed_constraints.emplace_back(
        MakeConstraint<sup::sequencer::Exists>(attr_def.GetName()).GetRepresentation());
      }
    }
  }
  return failed_constraints;
}

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

}  // unnamed namespace
