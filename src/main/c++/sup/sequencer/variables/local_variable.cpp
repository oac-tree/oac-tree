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

#include <sup/sequencer/log.h>
#include <sup/sequencer/procedure.h>

#include <sup/dto/anytype_helper.h>
#include <sup/dto/anyvalue_helper.h>

#include <memory>

namespace
{
std::string FullJSONRepresentation(const std::string& json_type, const std::string& json_value);
}  // unnamed namespace

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
  try
  {
    value = *m_value;
  }
  catch(const sup::dto::InvalidConversionException&)
  {
    return false;
  }
  return true;
}

bool LocalVariable::SetValueImpl(const sup::dto::AnyValue& value)
{
  try
  {
    *m_value = value;
    Notify(value);
  }
  catch(const sup::dto::InvalidConversionException&)
  {
    return false;
  }
  return true;
}

bool LocalVariable::SetupImpl(const sup::dto::AnyTypeRegistry& registry)
{
  m_value.reset(new sup::dto::AnyValue());

  if (HasAttribute(JSON_TYPE))
  {
    if (HasAttribute(JSON_VALUE))
    {
      try
      {
        *m_value = sup::dto::AnyValueFromJSONString(&registry,
          FullJSONRepresentation(GetAttribute(JSON_TYPE), GetAttribute(JSON_VALUE)));
      }
      catch(const sup::dto::ParseException&)
      {
        return false;
      }
    }
    else
    {
      try
      {
        sup::dto::AnyType parsed_type = sup::dto::AnyTypeFromJSONString(&registry,
                                                                        GetAttribute(JSON_TYPE));
        m_value.reset(new sup::dto::AnyValue(parsed_type));
      }
      catch (const sup::dto::ParseException&)
      {
        return false;
      }
    }
  }
  return true;  // empty AnyValue is allowed for setting
}

void LocalVariable::ResetImpl()
{
  m_value.reset();
}

}  // namespace sequencer

}  // namespace sup

namespace
{
std::string FullJSONRepresentation(const std::string& json_type, const std::string& json_value)
{
  const std::string prelude = R"RAW([{"encoding":"sup-dto/v1.0/JSON"},{"datatype":)RAW";
  const std::string interlude = R"RAW(},{"instance":)RAW";
  const std::string ending = R"RAW(}])RAW";
  return prelude + json_type + interlude + json_value + ending;
}
}  // unnamed namespace
