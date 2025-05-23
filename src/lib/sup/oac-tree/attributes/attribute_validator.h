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

#ifndef SUP_OAC_TREE_ATTRIBUTE_VALIDATOR_H_
#define SUP_OAC_TREE_ATTRIBUTE_VALIDATOR_H_

#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/constraint.h>

#include <sup/dto/anytype.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Class that handles the consistency of a set of attributes according to given constraints.
 */
class AttributeValidator
{
public:
  AttributeValidator();
  ~AttributeValidator();

  AttributeDefinition& AddAttributeDefinition(const std::string& attr_name,
                                              const sup::dto::AnyType& value_type);

  void AddConstraint(Constraint constraint);

  const std::vector<AttributeDefinition>& GetAttributeDefinitions() const;

  std::vector<std::string> ValidateAttributes(const StringAttributeList& str_attributes) const;

  std::pair<sup::dto::AnyValue, std::string> TryCreateAnyValue(
    const StringAttribute& str_attr) const;

private:
  using AttributeDefinitionIterator = std::vector<AttributeDefinition>::const_iterator;

  std::pair<sup::dto::AnyValue, std::string> TryCreateAnyValueImpl(
    const StringAttribute& str_attr, AttributeDefinitionIterator attr_def_it) const;
  bool HasAttributeDefinition(const std::string& attr_name) const;
  std::vector<AttributeDefinition> m_attribute_definitions;
  std::vector<Constraint> m_custom_constraints;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ATTRIBUTE_VALIDATOR_H_
