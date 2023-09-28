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

#ifndef SUP_SEQUENCER_ATTRIBUTE_PROPERTIES_H_
#define SUP_SEQUENCER_ATTRIBUTE_PROPERTIES_H_

#include <sup/dto/anytype.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace sequencer
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

  void SetMandatory(bool mandatory);

private:
  sup::dto::AnyType m_value_type;
  bool m_is_mandatory;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_PROPERTIES_H_
