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

#include "include.h"

#include "instruction_helper.h"

#include <sup/sequencer/parser/procedure_parser.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/procedure_context.h>
#include <sup/sequencer/procedure.h>

const std::string PATH_ATTRIBUTE_NAME = "path";
const std::string FILE_ATTRIBUTE_NAME = "file";

namespace sup
{
namespace sequencer
{
const std::string Include::Type = "Include";

Include::Include()
  : DecoratorInstruction(Include::Type)
{
  AddAttributeDefinition(PATH_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(FILE_ATTRIBUTE_NAME, sup::dto::StringType);
}

Include::~Include() = default;

void Include::SetupImpl(const Procedure& proc)
{
  auto proc_context = proc.GetContext();
  std::string proc_filename = proc_context.GetFilename();
  if (HasAttribute(FILE_ATTRIBUTE_NAME))
  {
    auto filename = GetAttributeValue<std::string>(FILE_ATTRIBUTE_NAME);
    proc_filename = GetFullPathName(GetFileDirectory(proc_filename), filename);
  }
  auto path = GetAttributeValue<std::string>(PATH_ATTRIBUTE_NAME);
  auto clone = proc_context.CloneInstructionPath(proc_filename, path);
  if (!clone)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "instruction not found, path: [" + path + "]";
    throw InstructionSetupException(error_message);
  }
  if (!InstructionHelper::InitialiseVariableAttributes(*clone, GetStringAttributes()))
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
