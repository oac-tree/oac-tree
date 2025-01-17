/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : File-based variable implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#include "file_variable.h"

#include <sup/oac-tree/concrete_constraints.h>
#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_value_parser.h>

#include <fstream>
#include <sstream>

namespace sup
{
namespace oac_tree
{
const std::string FileVariable::Type = "File";

FileVariable::FileVariable()
  : Variable(FileVariable::Type)
  , m_workspace_path{}
{
  AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(Constants::PRETTY_JSON_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

FileVariable::~FileVariable() = default;

bool FileVariable::GetValueImpl(sup::dto::AnyValue& value) const
{
  auto parsed_val = ReadValue();
  if (!parsed_val)
  {
    return false;
  }
  return sup::dto::TryAssign(value, *parsed_val);
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
    auto filename = GetFullPathName(m_workspace_path,
                                    GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME));
    sup::dto::AnyValueToJSONFile(value, filename, pretty_json);
    Notify(value, true);
  }
  catch(const sup::dto::SerializeException&)
  {
    return false;
  }
  return true;
}

SetupTeardownActions FileVariable::SetupImpl(const Workspace& ws)
{
  m_workspace_path = GetFileDirectory(ws.GetFilename());
  auto current_value = ReadValue();
  if (!current_value)
  {
    Notify({}, false);
  }
  else
  {
    Notify(*current_value, true);
  }
  return {};
}

bool FileVariable::IsAvailableImpl() const
{
  sup::dto::JSONAnyValueParser parser;
  auto filename = GetFullPathName(m_workspace_path,
                                  GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME));
  if (!parser.ParseFile(filename))
  {
    return false;
  }
  return true;
}

std::unique_ptr<sup::dto::AnyValue> FileVariable::ReadValue() const
{
  std::unique_ptr<sup::dto::AnyValue> result;
  sup::dto::JSONAnyValueParser parser;
  auto filename = GetFullPathName(m_workspace_path,
                                  GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME));
  if (!parser.ParseFile(filename))
  {
    return result;
  }
  result.reset(new sup::dto::AnyValue(parser.MoveAnyValue()));
  return result;
}

}  // namespace oac_tree

}  // namespace sup
