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

#include "instruction_helper.h"

#include <sup/sequencer/parser/procedure_parser.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure_context.h>
#include <sup/sequencer/sequence_parser.h>

const std::string FILE_ATTRIBUTE_NAME = "file";
const std::string PATH_ATTRIBUTE_NAME = "path";

namespace sup
{
namespace sequencer
{
const std::string IncludeProcedure::Type = "IncludeProcedure";

IncludeProcedure::IncludeProcedure()
  : DecoratorInstruction(IncludeProcedure::Type)
  , m_workspace{}
{
  AddAttributeDefinition(FILE_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(PATH_ATTRIBUTE_NAME, sup::dto::StringType);
}

IncludeProcedure::~IncludeProcedure() = default;

void IncludeProcedure::SetupImpl(const Procedure& proc)
{
  auto proc_context = proc.GetContext();
  std::string parent_proc_filename = proc_context.GetFilename();
  auto filename = GetAttributeValue<std::string>(FILE_ATTRIBUTE_NAME);
  auto proc_filename = GetFullPathName(GetFileDirectory(parent_proc_filename), filename);
  std::string path =
      HasAttribute(PATH_ATTRIBUTE_NAME) ? GetAttributeValue<std::string>(PATH_ATTRIBUTE_NAME) : "";
  auto instr_clone = proc_context.CloneInstructionPath(proc_filename, path);
  if (!instr_clone)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) + "instruction not found, "
      "path [" + path + "]";
    throw InstructionSetupException(error_message);
  }
  if (!InstructionHelper::InitialisePlaceholderAttributes(*instr_clone, GetStringAttributes()))
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "could not initialise variable attributes for child instruction(s)";
    throw InstructionSetupException(error_message);
  }
  (void)InsertInstruction(std::move(instr_clone), 0);
  m_workspace = proc_context.GetWorkspace(proc_filename);
  m_workspace->Setup();
  auto& sub_proc = proc_context.GetProcedure(proc_filename);
  SetupChild(sub_proc);
}

ExecutionStatus IncludeProcedure::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  (void)ws;
  auto child_status = GetChildStatus();
  if (NeedsExecute(child_status))
  {
    ExecuteChild(ui, *m_workspace);
  }
  return GetChildStatus();
}

bool IncludeProcedure::PostInitialiseVariables(const StringAttributeList& source_attributes)
{
  bool result = true;
  for (auto& attr : source_attributes)
  {
    // Do not propagate standard attributes of this instruction:
    if (IsDefinedAttributeName(*this, attr.first))
    {
      continue;
    }
    if (!HasAttribute(attr.first))
    {
      result = AddAttribute(attr.first, attr.second) && result;
    }
  }
  return result;
}

}  // namespace sequencer

}  // namespace sup
