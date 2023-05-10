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

#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/value_map_info.h>

#include <algorithm>

namespace
{
std::vector<sup::sequencer::AttributeDefinition>::const_iterator FindAttributeDefinition(
  const std::vector<sup::sequencer::AttributeDefinition>& attribute_definitions,
  const std::string& attr_name);

void RegisterFailedConstraints(const std::vector<sup::sequencer::Constraint>& constraints,
                               const sup::sequencer::StringAttributeList& str_attributes,
                               std::vector<std::string>& failed_constraints);
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
    // Definition found?
    //   N: add (name, AnyValue(str_value)) to result.value_map;
    //   Y: use type of definition to create the appropriate AnyValue value and
    //      add (name, value) to result.value_map
    //      if that failed, add failed type constraint representation to result.failed_constraints
    //      either way, remove the definition from the local list (attr_defs)
  }
  // Does attr_defs still contain mandatary attributes? If so, append their representation to
  // result.failed_constraints
  // Validate all custom constraints and append their representation to result.failed_constraints if
  // not successful
  // return a ValueMapInfo (leave value_map empty if there are failed constraints)
  return {};
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

void RegisterFailedConstraints(const std::vector<sup::sequencer::Constraint>& constraints,
                               const sup::sequencer::StringAttributeList& str_attributes,
                               std::vector<std::string>& failed_constraints)
{
  for (const auto& constraint : constraints)
  {
    if (!constraint.Validate(str_attributes))
    {
      failed_constraints.push_back(constraint.GetRepresentation());
    }
  }
}
}  // unnamed namespace
