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

#ifndef SUP_SEQUENCER_ATTRIBUTE_HANDLER2_H_
#define SUP_SEQUENCER_ATTRIBUTE_HANDLER2_H_

#include <sup/sequencer/attribute_validator.h>

namespace sup
{
namespace sequencer
{
struct ValueMapInfo;

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

  void AddConstraint(Constraint constraint);

  bool HasAttribute(const std::string& name) const;

  bool AddAttribute(const std::string& name, const std::string& value);

  const std::vector<AttributeDefinition>& GetAttributeDefinitions() const;

  ValueMapInfo CreateValueMap() const;

private:
  AttributeValidator m_attr_validator;
  StringAttributeList m_str_attributes;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_HANDLER2_H_
