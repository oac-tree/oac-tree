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

#include "include_procedure.h"

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
const std::string IncludeProcedure::Type = "IncludeProcedure";

IncludeProcedure::IncludeProcedure()
  : DecoratorInstruction(IncludeProcedure::Type)
  , m_workspace{}
{
  AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME).SetMandatory();
  AddAttributeDefinition(Constants::PATH_ATTRIBUTE_NAME);
}

IncludeProcedure::~IncludeProcedure() = default;

void IncludeProcedure::SetupImpl(const Procedure& proc)
{
  auto filename = GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME);
  auto proc_filename = ResolveRelativePath(proc, filename);
  auto proc_context = proc.GetContext();
  std::string path = GetAttributeString(Constants::PATH_ATTRIBUTE_NAME);
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
  m_workspace = std::addressof(proc_context.GetWorkspace(proc_filename));
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
  for (auto& [attr_name, attr_value] : source_attributes)
  {
    // Do not propagate standard attributes of this instruction:
    if (IsDefinedAttributeName(*this, attr_name))
    {
      continue;
    }
    if (!HasAttribute(attr_name))
    {
      result = AddAttribute(attr_name, attr_value) && result;
    }
  }
  return result;
}

}  // namespace oac_tree

}  // namespace sup
