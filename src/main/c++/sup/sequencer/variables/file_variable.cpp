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

#include "file_variable.h"

#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>

#include <fstream>
#include <sstream>

const std::string FILENAME_ATTR_NAME = "fileName";

namespace sup
{
namespace sequencer
{
const std::string FileVariable::Type = "File";

bool FileVariable::SetupImpl(const sup::dto::AnyTypeRegistry&)
{
  bool status = Variable::HasAttribute(FILENAME_ATTR_NAME);
  return status;
}

bool FileVariable::GetValueImpl(sup::dto::AnyValue& value) const
{
  try
  {
    sup::dto::AnyValue parsed_val = sup::dto::AnyValueFromJSONFile(GetAttribute(FILENAME_ATTR_NAME));
    return sup::dto::SafeAssign(value, parsed_val);
  }
  catch(const sup::dto::ParseException&)
  {
    return false;
  }
}

bool FileVariable::SetValueImpl(const sup::dto::AnyValue& value)
{
  try
  {
    sup::dto::AnyValueToJSONFile(value, GetAttribute(FILENAME_ATTR_NAME));
    Notify(value);
  }
  catch(const sup::dto::SerializeException&)
  {
    return false;
  }
  return true;
}

FileVariable::FileVariable() : Variable(FileVariable::Type) {}
FileVariable::~FileVariable() = default;

}  // namespace sequencer

}  // namespace sup
