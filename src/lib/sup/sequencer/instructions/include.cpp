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
  , m_filename{}
{
  AddAttributeDefinition(PATH_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(FILE_ATTRIBUTE_NAME, sup::dto::StringType);
}

Include::~Include() = default;

void Include::SetFilename(const std::string& filename)
{
  m_filename = filename;
}

std::string Include::GetFilename() const
{
  return m_filename;
}

ExecutionStatus Include::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (!HasChild())
  {
    return ExecutionStatus::SUCCESS;
  }
  auto child_status = GetChildStatus();
  if (NeedsExecute(child_status))
  {
    ExecuteChild(ui, ws);
  }
  return CalculateStatus();
}

bool Include::PostInitialiseVariables(const StringAttributeList& source_attributes)
{
  bool result = true;
  for (auto& attr : source_attributes)
  {
    if (!HasAttribute(attr.first))
    {
      result = AddAttribute(attr.first, attr.second) && result;
    }
  }
  return result;
}

void Include::SetupImpl(const Procedure& proc)
{
  std::string proc_filename = GetFilename();
  if (HasAttribute(FILE_ATTRIBUTE_NAME))
  {
    auto filename = GetAttributeValue<std::string>(FILE_ATTRIBUTE_NAME);
    proc_filename = GetFullPathName(GetFileDirectory(proc_filename), filename);
  }
  auto instructions = proc.GetInstructions(proc_filename);
  auto path = GetAttributeValue<std::string>(PATH_ATTRIBUTE_NAME);
  auto instr = InstructionHelper::FindInstruction(instructions, path);
  if (instr == nullptr)
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "instruction not found, path: [" + path + "]";
    throw InstructionSetupException(error_message);
  }
  auto clone = InstructionHelper::CloneInstruction(instr);
  if (!InstructionHelper::InitialiseVariableAttributes(*clone, GetStringAttributes()))
  {
    std::string error_message = InstructionSetupExceptionProlog(*this) +
      "could not initialise variable attributes for child instruction";
    throw InstructionSetupException(error_message);
  }
  (void)InsertInstruction(std::move(clone), 0);
  SetupChild(proc);
}

ExecutionStatus Include::CalculateStatus() const
{
  if (!HasChild())
  {
    return ExecutionStatus::SUCCESS;
  }
  return GetChildStatus();
}

}  // namespace sequencer

}  // namespace sup
