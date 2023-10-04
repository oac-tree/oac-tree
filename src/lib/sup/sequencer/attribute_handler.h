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

#ifndef SUP_SEQUENCER_ATTRIBUTE_HANDLER_H_
#define SUP_SEQUENCER_ATTRIBUTE_HANDLER_H_

#include <sup/sequencer/attribute_definition.h>
#include <sup/sequencer/constraint.h>
#include <sup/sequencer/exceptions.h>

#include <memory>

namespace sup
{
namespace sequencer
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

  sup::dto::AnyValue GetValue(const std::string& attr_name) const;

  std::pair<sup::dto::AnyValue, std::string> GetConvertedValue(
    const std::string& attr_name, const sup::dto::AnyValue& value) const;

  template <typename T>
  T GetValueAs(const std::string& attr_name) const;

private:
  StringAttribute GetStringAttribute(const std::string& attr_name) const;
  sup::dto::AnyValue TryCreateAnyValue(const StringAttribute& str_attr) const;
  struct AttributeHandlerImpl;
  std::unique_ptr<AttributeHandlerImpl> m_impl;
  StringAttributeList m_str_attributes;
};

template <typename T>
T AttributeHandler::GetValueAs(const std::string& attr_name) const
{
  auto val = GetValue(attr_name);
  T result;
  if (!val.As(result))
  {
    std::string message =
      "AttributeHandler::GetValueAs(): could not convert attribute with name [" + attr_name +
      "] to requested type";
    throw RuntimeException(message);
  }
  return result;
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

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_HANDLER_H_
