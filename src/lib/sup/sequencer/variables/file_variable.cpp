/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : File-based variable implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#include "file_variable.h"

#include <sup/sequencer/concrete_constraints.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_value_parser.h>

#include <fstream>
#include <sstream>

const std::string FILENAME_ATTR_NAME = "fileName";
const std::string PRETTY_JSON_ATTR_NAME = "pretty";

namespace sup
{
namespace sequencer
{
const std::string FileVariable::Type = "File";

FileVariable::FileVariable()
  : Variable(FileVariable::Type)
{
  AddAttributeDefinition(FILENAME_ATTR_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(PRETTY_JSON_ATTR_NAME, sup::dto::BooleanType);
  AddConstraint(MakeConstraint<NonEmpty>(FILENAME_ATTR_NAME));
}

FileVariable::~FileVariable() = default;

bool FileVariable::GetValueImpl(sup::dto::AnyValue& value) const
{
  sup::dto::JSONAnyValueParser parser;
  if (!parser.ParseFile(GetAttributeValue<std::string>(FILENAME_ATTR_NAME)))
  {
    return false;
  }
  auto parsed_val = parser.MoveAnyValue();
  if (!sup::dto::IsEmptyValue(value) && value.GetType() != parsed_val.GetType())
  {
    return false;
  }
  value = parsed_val;
  return true;
}

bool FileVariable::SetValueImpl(const sup::dto::AnyValue& value)
{
  bool pretty_json = false;
  if (HasAttribute(PRETTY_JSON_ATTR_NAME))
  {
    pretty_json = GetAttributeValue<bool>(PRETTY_JSON_ATTR_NAME);
  }
  try
  {
    sup::dto::AnyValueToJSONFile(value, GetAttributeValue<std::string>(FILENAME_ATTR_NAME),
                                 pretty_json);
    Notify(value, true);
  }
  catch(const sup::dto::SerializeException&)
  {
    return false;
  }
  return true;
}

}  // namespace sequencer

}  // namespace sup
