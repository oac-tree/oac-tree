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

#include "local_variable.h"

#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

#include <memory>

namespace sup
{
namespace sequencer
{

const std::string LocalVariable::Type = "Local";

const std::string JSON_TYPE_ATTRIBUTE = "type";
const std::string JSON_VALUE_ATTRIBUTE = "value";
const std::string DYNAMIC_TYPE_ATTRIBUTE = "dynamicType";

LocalVariable::LocalVariable()
  : Variable(LocalVariable::Type)
  , m_value{}
{
  AddAttributeDefinition(JSON_TYPE_ATTRIBUTE, sup::dto::StringType);
  AddAttributeDefinition(JSON_VALUE_ATTRIBUTE, sup::dto::StringType);
  AddAttributeDefinition(DYNAMIC_TYPE_ATTRIBUTE, sup::dto::BooleanType);
  AddConstraint(MakeConstraint<Or>(
                  MakeConstraint<Exists>(JSON_TYPE_ATTRIBUTE),
                  MakeConstraint<Not>(MakeConstraint<Exists>(JSON_VALUE_ATTRIBUTE))));
}

LocalVariable::~LocalVariable() {}

bool LocalVariable::IsDynamicallyTyped() const
{
  bool is_dynamic = false;
  if (!GetAttributeValue(DYNAMIC_TYPE_ATTRIBUTE, is_dynamic))
  {
    return false;
  }
  return is_dynamic;
}

bool LocalVariable::GetValueImpl(sup::dto::AnyValue& value) const
{
  if (sup::dto::IsEmptyValue(m_value))
  {
    return false;
  }
  return sup::dto::TryAssign(value, m_value);
}

bool LocalVariable::SetValueImpl(const sup::dto::AnyValue& value)
{
  bool result = false;
  if (IsDynamicallyTyped())
  {
    result = sup::dto::TryAssign(m_value, value);
  }
  else
  {
    result = sup::dto::TryAssignIfEmptyOrConvert(m_value, value);
  }
  if (result)
  {
    Notify(value, true);
  }
  return result;
}

void LocalVariable::SetupImpl(const sup::dto::AnyTypeRegistry& registry)
{
  m_value = ParseAnyValueAttributePair(*this, JSON_TYPE_ATTRIBUTE, JSON_VALUE_ATTRIBUTE, registry);
}

void LocalVariable::ResetImpl()
{
  m_value = sup::dto::AnyValue{};
}

}  // namespace sequencer

}  // namespace sup
