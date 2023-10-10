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

#include "copy_from_procedure.h"

#include "instruction_helper.h"

#include <sup/sequencer/parser/procedure_parser.h>

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure_context.h>
#include <sup/sequencer/sequence_parser.h>

namespace sup
{
namespace sequencer
{
const std::string CopyFromProcedureInstruction::Type = "CopyFromProcedure";

CopyFromProcedureInstruction::CopyFromProcedureInstruction()
  : Instruction(CopyFromProcedureInstruction::Type)
  , m_workspace{}
{
  AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME).SetMandatory();
  AddAttributeDefinition(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

CopyFromProcedureInstruction::~CopyFromProcedureInstruction() = default;

void CopyFromProcedureInstruction::SetupImpl(const Procedure& proc)
{
  auto proc_context = proc.GetContext();
  std::string parent_proc_filename = proc_context.GetFilename();
  auto filename = GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME);
  auto proc_filename = GetFullPathName(GetFileDirectory(parent_proc_filename), filename);
  m_workspace = proc_context.GetWorkspace(proc_filename);
  m_workspace->Setup();
}

ExecutionStatus CopyFromProcedureInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME, *m_workspace, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME,
                                 value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
