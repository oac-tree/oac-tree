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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_ATTRIBUTE_HANDLER_H_
#define SUP_OAC_TREE_ATTRIBUTE_HANDLER_H_

#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/constraint.h>
#include <sup/oac-tree/exceptions.h>

#include <memory>

namespace sup
{
namespace oac_tree
{
class Workspace;

/**
 * @brief Provides information on an attributes value: if m_is_varname is true, m_value contains
 * a workspace variable name; if not, it contains a string to be interpreted as a literal value.
 */
struct AttributeValueInfo
{
  bool m_is_varname;
  std::string m_value;
};

/**
 * @brief Class that handles the consistency of a set of attributes according to given constraints.
 */
class AttributeHandler
{
public:
  AttributeHandler();
  ~AttributeHandler();

  AttributeDefinition& AddAttributeDefinition(const std::string& attr_name,
                                              const sup::dto::AnyType& value_type);

  const std::vector<AttributeDefinition>& GetAttributeDefinitions() const;

  void AddConstraint(Constraint constraint);

  bool HasStringAttribute(const std::string& name) const;

  bool AddStringAttribute(const std::string& name, const std::string& value);

  void SetStringAttribute(const std::string& name, const std::string& value);

  const StringAttributeList& GetStringAttributes() const;

  bool ValidateAttributes();

  void ClearFailedConstraints();

  std::vector<std::string> GetFailedConstraints() const;

  bool GetValue(const std::string& attr_name, sup::dto::AnyValue& value) const;

  template <typename T>
  bool GetValueAs(const std::string& attr_name, T& val) const;

private:
  struct AttributeHandlerImpl;
  std::unique_ptr<AttributeHandlerImpl> m_impl;
  StringAttributeList m_str_attributes;
};

template <typename T>
bool AttributeHandler::GetValueAs(const std::string& attr_name, T& val) const
{
  sup::dto::AnyValue anyvalue;
  if (!GetValue(attr_name, anyvalue))
  {
    return false;
  }
  if (!anyvalue.As(val))
  {
    return false;
  }
  return true;
}

std::string GetStringAttributeValue(const StringAttributeList& str_attributes,
                                    const std::string& attr_name);

StringAttributeList::iterator FindStringAttribute(StringAttributeList& str_attributes,
                                                  const std::string& attr_name);

StringAttributeList::const_iterator FindStringAttribute(const StringAttributeList& str_attributes,
                                                        const std::string& attr_name);

std::string FormatFailedConstraints(const std::vector<std::string>& failed_constraints,
                                    const std::string& prefix = "\n");

AttributeValueInfo GetAttributeValueInfo(const StringAttributeList& str_attributes,
                                         const std::vector<AttributeDefinition>& attr_defs,
                                         const std::string& attr_name);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ATTRIBUTE_HANDLER_H_
