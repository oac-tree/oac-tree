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

#include "include_procedure.h"

#include <sup/sequencer/parser/procedure_parser.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/sequence_parser.h>

const std::string FILE_ATTRIBUTE_NAME = "file";
const std::string PATH_ATTRIBUTE_NAME = "path";

namespace sup
{
namespace sequencer
{
const std::string IncludeProcedure::Type = "IncludeProcedure";

IncludeProcedure::IncludeProcedure()
  : Instruction(IncludeProcedure::Type)
  , m_root_instruction{}
  , m_workspace{}
{
  AddAttributeDefinition(FILE_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(PATH_ATTRIBUTE_NAME, sup::dto::StringType);
}

IncludeProcedure::~IncludeProcedure() = default;

void IncludeProcedure::SetupImpl(const Procedure& proc)
{
  std::string parent_proc_filename = proc.GetFilename();
  auto filename = GetAttributeValue<std::string>(FILE_ATTRIBUTE_NAME);
  auto proc_filename = GetFullPathName(GetFileDirectory(parent_proc_filename), filename);
  auto& sub_proc = proc.GetSubProcedure(proc_filename);
  std::string path =
      HasAttribute(PATH_ATTRIBUTE_NAME) ? GetAttributeValue<std::string>(PATH_ATTRIBUTE_NAME) : "";
  m_root_instruction = CloneInstructionPath(sub_proc, path);
  if (!m_root_instruction)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) + "instruction not found";
    throw InstructionSetupException(error_message);
  }
  m_workspace = proc.GetSubWorkspace(proc_filename);
}

ExecutionStatus IncludeProcedure::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ws;
  m_root_instruction->ExecuteSingle(ui, *m_workspace);
  return m_root_instruction->GetStatus();
}

}  // namespace sequencer

}  // namespace sup
