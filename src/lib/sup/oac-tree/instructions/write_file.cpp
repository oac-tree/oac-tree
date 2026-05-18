/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "write_file.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/procedure.h>

#include <sup/dto/anyvalue_helper.h>

namespace sup
{
namespace oac_tree
{
const std::string WriteFile::Type = "WriteFile";

WriteFile::WriteFile()
  : Instruction(WriteFile::Type)
  , m_proc_path{}
{
  (void)AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  (void)AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME, sup::dto::StringType)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  (void)AddAttributeDefinition(Constants::PRETTY_JSON_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

WriteFile::~WriteFile() = default;

void WriteFile::SetupImpl(const Procedure& proc)
{
  m_proc_path = GetProcedurePath(proc);
}

ExecutionStatus WriteFile::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  bool pretty_json = false;
  if (!GetAttributeValueAs(Constants::PRETTY_JSON_ATTRIBUTE_NAME, ws, ui, pretty_json))
  {
    return ExecutionStatus::FAILURE;
  }
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  std::string local_filename{};
  if (!GetAttributeValueAs(Constants::FILENAME_ATTRIBUTE_NAME, ws, ui, local_filename))
  {
    return ExecutionStatus::FAILURE;
  }
  auto filename = GetFullPathName(m_proc_path, local_filename);
  try
  {
    sup::dto::AnyValueToJSONFile(value, filename, pretty_json);
  }
  catch (const sup::dto::SerializeException&)
  {
    const std::string warning =
        InstructionWarningProlog(*this) + " could not write to file with name: " + filename;
    LogWarning(ui, warning);
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
