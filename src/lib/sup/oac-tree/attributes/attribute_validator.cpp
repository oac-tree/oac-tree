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

#include "attribute_validator.h"

#include <sup/oac-tree/attribute_handler.h>
#include <sup/oac-tree/attribute_utils.h>
#include <sup/oac-tree/concrete_constraints.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>

#include <sup/oac-tree/instructions/instruction_helper.h>

#include <sup/dto/anyvalue_helper.h>

#include <algorithm>

namespace
{
using sup::oac_tree::AttributeDefinition;
using sup::oac_tree::StringAttribute;
using sup::oac_tree::StringAttributeList;
using sup::oac_tree::Constraint;

std::vector<std::string> CheckMandatoryConstraints(
  const std::vector<AttributeDefinition>& attr_defs, const StringAttributeList& str_attributes);

std::vector<AttributeDefinition>::const_iterator FindAttributeDefinition(
  const std::vector<AttributeDefinition>& attribute_definitions,
  const std::string& attr_name);

bool AttributeRefersToVariable(const AttributeDefinition& attr_def, const std::string& attr_val);
}  // unnamed namespace

namespace sup
{
namespace oac_tree
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
  for (const auto& [attr_name, attr_value] : str_attributes)
  {
    auto it = FindAttributeDefinition(m_attribute_definitions, attr_name);
    if (it != m_attribute_definitions.end() &&
        AttributeRefersToVariable(*it, attr_value))
    {
      continue;  // Don't validate attribute values referring to workspace variables
    }
    auto [_, constraint] = TryCreateAnyValueImpl({attr_name, attr_value}, it);
    if (!constraint.empty())
    {
      failed_constraints.push_back(constraint);
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
  return TryCreateAnyValueImpl(str_attr, it);
}

std::pair<sup::dto::AnyValue, std::string> AttributeValidator::TryCreateAnyValueImpl(
    const StringAttribute& str_attr, AttributeDefinitionIterator attr_def_it) const
{
  auto [attr_name, attr_value] = str_attr;
  if (attr_def_it == m_attribute_definitions.end())
  {
    return { sup::dto::AnyValue(attr_value), "" };
  }
  auto attr_type = attr_def_it->GetType();
  auto [parsed, value] = utils::ParseAttributeString(attr_type, attr_value);
  if (parsed)
  {
    return { value, "" };
  }
  auto failed_constraint =
    MakeConstraint<FixedType>(attr_name, attr_type).GetRepresentation();
  return { {}, failed_constraint };
}

bool AttributeValidator::HasAttributeDefinition(const std::string& attr_name) const
{
  auto it = FindAttributeDefinition(m_attribute_definitions, attr_name);
  return it != m_attribute_definitions.end();
}

}  // namespace oac_tree

}  // namespace sup

namespace
{

std::vector<std::string> CheckMandatoryConstraints(
  const std::vector<AttributeDefinition>& attr_defs, const StringAttributeList& str_attributes)
{
  using sup::oac_tree::MakeConstraint;
  std::vector<std::string> failed_constraints;
  for (const auto& attr_def : attr_defs)
  {
    if (attr_def.IsMandatory())
    {
      auto it = sup::oac_tree::FindStringAttribute(str_attributes, attr_def.GetName());
      if (it == str_attributes.end())
      {
      failed_constraints.emplace_back(
        MakeConstraint<sup::oac_tree::Exists>(attr_def.GetName()).GetRepresentation());
      }
    }
  }
  return failed_constraints;
}

std::vector<sup::oac_tree::AttributeDefinition>::const_iterator FindAttributeDefinition(
  const std::vector<sup::oac_tree::AttributeDefinition>& attribute_definitions,
  const std::string& attr_name)
{
  auto predicate = [&attr_name](const sup::oac_tree::AttributeDefinition& attr_def)
                   {
                     return attr_def.GetName() == attr_name;
                   };
  return std::find_if(attribute_definitions.begin(), attribute_definitions.end(), predicate);
}

bool AttributeRefersToVariable(const AttributeDefinition& attr_def, const std::string& attr_val)
{
  using sup::oac_tree::AttributeCategory;
  using sup::oac_tree::InstructionHelper::AttributeStartsWith;
  const auto attr_cat = attr_def.GetCategory();
  if (attr_cat == AttributeCategory::kVariableName)
  {
    return true;
  }
  if (attr_cat == AttributeCategory::kBoth &&
      AttributeStartsWith(attr_val, sup::oac_tree::DefaultSettings::VARIABLE_ATTRIBUTE_CHAR))
  {
    return true;
  }
  return false;
}

}  // unnamed namespace
