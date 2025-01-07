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

#ifndef SUP_SEQUENCER_ATTRIBUTE_DEFINITION_H_
#define SUP_SEQUENCER_ATTRIBUTE_DEFINITION_H_

#include <sup/dto/anytype.h>

#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
class AttributeProperties;

/**
 * @brief Category of an attribute indicating if the attribute's value needs to be interpreted as
 * a literal (typed) value, the name of a workspace variable that contains the value or both. When
 * the category is `kBoth`, an `@` prefix in the value indicates this refers to a workspace
 * variable.
 *
 * @note The default category for an attribute is `kLiteral`.
 */
enum class AttributeCategory
{
  kLiteral = 0,
  kVariableName,
  kBoth
};

/**
 * @brief Class that defines a specific attribute by name and type.
 */
class AttributeDefinition
{
public:
  AttributeDefinition(const std::string& name, const sup::dto::AnyType& value_type);
  ~AttributeDefinition();

  AttributeDefinition(const AttributeDefinition& other);
  AttributeDefinition(AttributeDefinition&& other) noexcept;

  AttributeDefinition& operator=(const AttributeDefinition& other) &;
  AttributeDefinition& operator=(AttributeDefinition&& other) & noexcept;

  std::string GetName() const;
  sup::dto::AnyType GetType() const;
  bool IsMandatory() const;
  AttributeCategory GetCategory() const;

  AttributeDefinition& SetMandatory(bool mandatory = true);
  AttributeDefinition& SetCategory(AttributeCategory cat);

private:
  std::string m_name;
  std::unique_ptr<AttributeProperties> m_properties;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_DEFINITION_H_
