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

#ifndef SUP_OAC_TREE_ATTRIBUTE_PROPERTIES_H_
#define SUP_OAC_TREE_ATTRIBUTE_PROPERTIES_H_

#include <sup/oac-tree/attribute_definition.h>

#include <sup/dto/anytype.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{

/**
 * @brief Class contains the different properties of an attribute.
 */
class AttributeProperties
{
public:
  AttributeProperties(const sup::dto::AnyType& value_type);
  ~AttributeProperties();

  AttributeProperties(const AttributeProperties& other) = default;
  AttributeProperties(AttributeProperties&& other) = default;

  AttributeProperties& operator=(const AttributeProperties& other) = default;
  AttributeProperties& operator=(AttributeProperties&& other) = default;

  sup::dto::AnyType GetType() const;

  bool IsMandatory() const;

  AttributeCategory GetCategory() const;

  void SetMandatory(bool mandatory);

  void SetCategory(AttributeCategory cat);

private:
  sup::dto::AnyType m_value_type;
  bool m_is_mandatory;
  AttributeCategory m_category;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ATTRIBUTE_PROPERTIES_H_
