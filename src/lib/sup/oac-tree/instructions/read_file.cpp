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

#include "read_file.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction_utils.h>
#include <sup/oac-tree/procedure.h>

namespace sup
{
namespace oac_tree
{
const std::string ReadFile::Type = "ReadFile";

ReadFile::ReadFile()
  : Instruction(ReadFile::Type)
  , m_proc_path{}
{
  (void)AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME, sup::dto::StringType)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
  (void)AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

ReadFile::~ReadFile() = default;

void ReadFile::SetupImpl(const Procedure& proc)
{
  m_proc_path = GetProcedurePath(proc);
}

ExecutionStatus ReadFile::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  std::string local_filename{};
  if (!GetAttributeValueAs(Constants::FILENAME_ATTRIBUTE_NAME, ws, ui, local_filename))
  {
    return ExecutionStatus::FAILURE;
  }
  auto filename = GetFullPathName(m_proc_path, local_filename);
  auto value = instruction_utils::ReadFile(filename);
  if (!value)
  {
    const std::string warning =
        InstructionWarningProlog(*this) + " could not parse file with name: " + filename;
    LogWarning(ui, warning);
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME,
                                 *value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
