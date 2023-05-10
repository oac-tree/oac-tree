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

#include <algorithm>

namespace
{
void RegisterFailedConstraints(const std::vector<sup::sequencer::Constraint>& constraints,
                               const sup::sequencer::ValueMap& attr_map,
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

std::vector<std::string> AttributeValidator::FailedConstraints(const ValueMap& attr_map) const
{
  std::vector<std::string> failed_constraints;
  auto simple_constraints = GetSimpleConstraints();
  RegisterFailedConstraints(simple_constraints, attr_map, failed_constraints);
  RegisterFailedConstraints(m_custom_constraints, attr_map, failed_constraints);
  return failed_constraints;
}

bool AttributeValidator::HasAttributeDefinition(const std::string& attr_name) const
{
  auto it = std::find_if(m_attribute_definitions.begin(), m_attribute_definitions.end(),
                         [&attr_name](const AttributeDefinition& attr_def){
                           return attr_def.GetName() == attr_name;
                         });
  return it != m_attribute_definitions.end();
}

std::vector<Constraint> AttributeValidator::GetSimpleConstraints() const
{
  std::vector<Constraint> result;
  for (const auto& attr_def : m_attribute_definitions)
  {
    auto attr_name = attr_def.GetName();
    if (attr_def.IsMandatory())
    {
      result.push_back(MakeConstraint<Exists>(attr_name));
    }
    auto attr_type = attr_def.GetType();
    if (!sup::dto::IsEmptyType(attr_type))
    {
      result.push_back(MakeConstraint<FixedType>(attr_name, attr_type));
    }
  }
  return result;
}

}  // namespace sequencer

}  // namespace sup

namespace
{
void RegisterFailedConstraints(const std::vector<sup::sequencer::Constraint>& constraints,
                               const sup::sequencer::ValueMap& attr_map,
                               std::vector<std::string>& failed_constraints)
{
  for (const auto& constraint : constraints)
  {
    if (!constraint.Validate(attr_map))
    {
      failed_constraints.push_back(constraint.GetRepresentation());
    }
  }
}
}  // unnamed namespace
