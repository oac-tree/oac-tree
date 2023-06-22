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

#include <memory>

namespace sup
{
namespace sequencer
{

const std::string LocalVariable::Type = "Local";

const std::string JSON_TYPE = "type";
const std::string JSON_VALUE = "value";

LocalVariable::LocalVariable()
  : Variable(LocalVariable::Type)
  , m_value{}
{
  AddAttributeDefinition(JSON_TYPE, sup::dto::StringType);
  AddAttributeDefinition(JSON_VALUE, sup::dto::StringType);
  AddConstraint(MakeConstraint<Or>(MakeConstraint<Exists>(JSON_TYPE),
                                   MakeConstraint<Not>(MakeConstraint<Exists>(JSON_VALUE))));
}

LocalVariable::~LocalVariable() {}

bool LocalVariable::GetValueImpl(sup::dto::AnyValue& value) const
{
  if (sup::dto::IsEmptyValue(*m_value))
  {
    return false;
  }
  if (!sup::dto::IsEmptyValue(value) && value.GetType() != m_value->GetType())
  {
    return false;
  }
  value = *m_value;
  return true;
}

bool LocalVariable::SetValueImpl(const sup::dto::AnyValue& value)
{
  if (!sup::dto::IsEmptyValue(*m_value) && value.GetType() != m_value->GetType())
  {
    return false;
  }
  *m_value = value;
  Notify(value, true);
  return true;
}

void LocalVariable::SetupImpl(const sup::dto::AnyTypeRegistry& registry)
{
  auto parsed_value = ParseAnyValueAttributePair(*this, JSON_TYPE, JSON_VALUE, registry);
  m_value.reset(new sup::dto::AnyValue{parsed_value});
}

void LocalVariable::ResetImpl()
{
  m_value.reset();
}

}  // namespace sequencer

}  // namespace sup
