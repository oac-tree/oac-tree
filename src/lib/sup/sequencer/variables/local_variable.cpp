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
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/json_type_parser.h>
#include <sup/dto/json_value_parser.h>

#include <memory>

namespace sup
{
namespace sequencer
{

const std::string LocalVariable::Type = "Local";
const std::string LocalVariable::JSON_TYPE = "type";
const std::string LocalVariable::JSON_VALUE = "value";

LocalVariable::LocalVariable()
  : Variable(LocalVariable::Type)
  , m_value{}
{}

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
  Notify(value);
  return true;
}

void LocalVariable::SetupImpl(const sup::dto::AnyTypeRegistry& registry)
{
  m_value.reset(new sup::dto::AnyValue());

  // empty AnyValue is allowed for setting
  if (HasAttribute(JSON_TYPE))
  {
    auto type_str = GetAttribute(JSON_TYPE);
    sup::dto::JSONAnyTypeParser type_parser;
    if (!type_parser.ParseString(type_str, &registry))
    {
      std::string error_message = VariableSetupExceptionProlog() +
        "could not parse attribute [" + JSON_TYPE + "] with value [" + type_str + "]";
      throw VariableSetupException(error_message);
    }
    auto parsed_type = type_parser.MoveAnyType();
    if (HasAttribute(JSON_VALUE))
    {
      auto val_str = GetAttribute(JSON_VALUE);
      sup::dto::JSONAnyValueParser value_parser;
      if (!value_parser.TypedParseString(parsed_type, val_str))
      {
      std::string error_message = VariableSetupExceptionProlog() +
        "could not parse attribute [" + JSON_VALUE + "] with value [" + val_str +
        "]";
      throw VariableSetupException(error_message);
      }
      m_value.reset(new sup::dto::AnyValue(value_parser.MoveAnyValue()));
    }
    else
    {
      m_value.reset(new sup::dto::AnyValue(parsed_type));
    }
  }
  else if (HasAttribute(JSON_VALUE))
  {
    std::string error_message = VariableSetupExceptionProlog() +
      "attribute [" + JSON_VALUE + "] present without attribute [" + JSON_TYPE + "]";
    throw VariableSetupException(error_message);
  }
}

void LocalVariable::ResetImpl()
{
  m_value.reset();
}

}  // namespace sequencer

}  // namespace sup
