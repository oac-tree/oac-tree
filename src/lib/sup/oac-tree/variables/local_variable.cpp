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

#include "local_variable.h"

#include <sup/oac-tree/concrete_constraints.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

#include <memory>

namespace sup
{
namespace oac_tree
{

const std::string LocalVariable::Type = "Local";

LocalVariable::LocalVariable()
  : Variable(LocalVariable::Type)
  , m_value{}
{
  AddAttributeDefinition(Constants::TYPE_ATTRIBUTE_NAME, sup::dto::StringType);
  AddAttributeDefinition(Constants::VALUE_ATTRIBUTE_NAME, sup::dto::StringType);
  AddAttributeDefinition(Constants::IS_DYNAMIC_TYPE_ATTRIBUTE_NAME, sup::dto::BooleanType);
  AddConstraint(MakeConstraint<Or>(
                  MakeConstraint<Exists>(Constants::TYPE_ATTRIBUTE_NAME),
                  MakeConstraint<Not>(MakeConstraint<Exists>(Constants::VALUE_ATTRIBUTE_NAME))));
}

LocalVariable::~LocalVariable() {}

bool LocalVariable::IsDynamicallyTyped() const
{
  bool is_dynamic = false;
  if (!GetAttributeValue(Constants::IS_DYNAMIC_TYPE_ATTRIBUTE_NAME, is_dynamic))
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

SetupTeardownActions LocalVariable::SetupImpl(const Workspace& ws)
{
  m_value = ParseAnyValueAttributePair(
    *this, Constants::TYPE_ATTRIBUTE_NAME, Constants::VALUE_ATTRIBUTE_NAME, ws.GetTypeRegistry());
  Notify(m_value, true);
  return {};
}

void LocalVariable::TeardownImpl()
{
  m_value = sup::dto::AnyValue{};
}

}  // namespace oac_tree

}  // namespace sup
