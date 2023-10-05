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

#include "copy_to_procedure.h"

#include "instruction_helper.h"

#include <sup/sequencer/parser/procedure_parser.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure_context.h>
#include <sup/sequencer/sequence_parser.h>

const std::string FILE_ATTRIBUTE_NAME = "file";
const std::string INPUT_VARIABLE_ATTR_NAME = "inputVar";
const std::string OUTPUT_VARIABLE_ATTR_NAME = "outputVar";

namespace sup
{
namespace sequencer
{
const std::string CopyToProcedureInstruction::Type = "CopyToProcedure";

CopyToProcedureInstruction::CopyToProcedureInstruction()
  : Instruction(CopyToProcedureInstruction::Type)
  , m_workspace{}
{
  AddAttributeDefinition(FILE_ATTRIBUTE_NAME).SetMandatory();
  AddAttributeDefinition(INPUT_VARIABLE_ATTR_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
  AddAttributeDefinition(OUTPUT_VARIABLE_ATTR_NAME)
    .SetCategory(AttributeCategory::kVariableName).SetMandatory();
}

CopyToProcedureInstruction::~CopyToProcedureInstruction() = default;

void CopyToProcedureInstruction::SetupImpl(const Procedure& proc)
{
  auto proc_context = proc.GetContext();
  std::string parent_proc_filename = proc_context.GetFilename();
  auto filename = GetAttributeValue<std::string>(FILE_ATTRIBUTE_NAME);
  auto proc_filename = GetFullPathName(GetFileDirectory(parent_proc_filename), filename);
  m_workspace = proc_context.GetWorkspace(proc_filename);
  m_workspace->Setup();
}

ExecutionStatus CopyToProcedureInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetAttributeValue(INPUT_VARIABLE_ATTR_NAME, ws, ui, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, *m_workspace, ui, OUTPUT_VARIABLE_ATTR_NAME, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
