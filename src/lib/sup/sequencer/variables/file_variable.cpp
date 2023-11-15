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
#include <sup/sequencer/constants.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_value_parser.h>

#include <fstream>
#include <sstream>

namespace sup
{
namespace sequencer
{
const std::string FileVariable::Type = "File";

FileVariable::FileVariable()
  : Variable(FileVariable::Type)
{
  AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(Constants::PRETTY_JSON_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

FileVariable::~FileVariable() = default;

bool FileVariable::GetValueImpl(sup::dto::AnyValue& value) const
{
  sup::dto::JSONAnyValueParser parser;
  if (!parser.ParseFile(GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME)))
  {
    return false;
  }
  auto parsed_val = parser.MoveAnyValue();
  return sup::dto::TryAssign(value, parsed_val);
}

bool FileVariable::SetValueImpl(const sup::dto::AnyValue& value)
{
  bool pretty_json = false;
  if (!GetAttributeValue(Constants::PRETTY_JSON_ATTRIBUTE_NAME, pretty_json))
  {
    return false;
  }
  try
  {
    sup::dto::AnyValueToJSONFile(value, GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME),
                                 pretty_json);
    Notify(value, true);
  }
  catch(const sup::dto::SerializeException&)
  {
    return false;
  }
  return true;
}
bool FileVariable::IsAvailableImpl() const
{
  sup::dto::JSONAnyValueParser parser;
  if (!parser.ParseFile(GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME)))
  {
    return false;
  }
  return true;
}
}  // namespace sequencer

}  // namespace sup
