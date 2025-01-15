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

#include "include.h"

#include "instruction_helper.h"

#include <sup/sequencer/parser/procedure_parser.h>

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure_context.h>
#include <sup/sequencer/procedure.h>

namespace sup
{
namespace sequencer
{
const std::string Include::Type = "Include";

Include::Include()
  : DecoratorInstruction(Include::Type)
{
  AddAttributeDefinition(Constants::PATH_ATTRIBUTE_NAME).SetMandatory();
  AddAttributeDefinition(Constants::FILENAME_ATTRIBUTE_NAME);
}

Include::~Include() = default;

void Include::SetupImpl(const Procedure& proc)
{
  std::string proc_filename = proc.GetFilename();
  if (HasAttribute(Constants::FILENAME_ATTRIBUTE_NAME))
  {
    auto filename = GetAttributeString(Constants::FILENAME_ATTRIBUTE_NAME);
    proc_filename = ResolveRelativePath(proc, filename);
  }
  auto proc_context = proc.GetContext();
  auto path = GetAttributeString(Constants::PATH_ATTRIBUTE_NAME);
  auto clone = proc_context.CloneInstructionPath(proc_filename, path);
  if (!clone)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "instruction not found, path: [" + path + "]";
    throw InstructionSetupException(error_message);
  }
  if (!InstructionHelper::InitialisePlaceholderAttributes(*clone, GetStringAttributes()))
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "could not initialise variable attributes for child instruction(s)";
    throw InstructionSetupException(error_message);
  }
  (void)InsertInstruction(std::move(clone), 0);
  auto& sub_proc = proc_context.GetProcedure(proc_filename);
  SetupChild(sub_proc);
}

ExecutionStatus Include::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto child_status = GetChildStatus();
  if (NeedsExecute(child_status))
  {
    ExecuteChild(ui, ws);
  }
  return GetChildStatus();
}

bool Include::PostInitialiseVariables(const StringAttributeList& source_attributes)
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

}  // namespace sequencer

}  // namespace sup
