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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "copy_to_procedure.h"

#include "instruction_helper.h"

#include <sup/oac-tree/parser/procedure_parser.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/procedure_context.h>
#include <sup/oac-tree/sequence_parser.h>

namespace sup
{
namespace oac_tree
{
const std::string CopyToProcedureInstruction::Type = "CopyToProcedure";

CopyToProcedureInstruction::CopyToProcedureInstruction()
  : Instruction(CopyToProcedureInstruction::Type)
  , m_workspace{}
{
  AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME).SetMandatory();
  AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

CopyToProcedureInstruction::~CopyToProcedureInstruction() = default;

void CopyToProcedureInstruction::SetupImpl(const Procedure& proc)
{
  auto filename = GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME);
  auto proc_filename = ResolveRelativePath(proc, filename);
  auto proc_context = proc.GetContext();
  m_workspace = std::addressof(proc_context.GetWorkspace(proc_filename));
  m_workspace->Setup();
}

ExecutionStatus CopyToProcedureInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, *m_workspace, ui,
                                 Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace oac_tree

}  // namespace sup
