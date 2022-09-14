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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files

#include <sup/sequencer/log.h>

// Local header files

#include "Include.h"
#include "InstructionHelper.h"
#include <sup/sequencer/procedure.h>
#include "procedure_parser.h"

// Constants

static const std::string PATH_ATTRIBUTE_NAME = "path";
static const std::string FILE_ATTRIBUTE_NAME = "file";

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

const std::string Include::Type = "Include";

// Function declaration

// Function definition

ExecutionStatus Include::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
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

bool Include::PostInitialiseVariables(const AttributeMap& source)
{
  bool result = true;
  for (auto& attr : source)
  {
    if (!HasAttribute(attr.first))
    {
      result = AddAttribute(attr.first, attr.second) && result;
    }
  }
  return result;
}

bool Include::SetupImpl(const Procedure& proc)
{
  std::string proc_filename = GetFilename();
  if (HasAttribute(FILE_ATTRIBUTE_NAME))
  {
    auto filename = GetAttribute(FILE_ATTRIBUTE_NAME);
    proc_filename = GetFullPathName(GetFileDirectory(proc_filename), filename);
  }
  auto instructions = proc.GetInstructions(proc_filename);
  auto path = GetPath();
  auto instr = InstructionHelper::FindInstruction(instructions, path);
  if (instr == nullptr)
  {
    log::Warning("Include::SetupImpl(): instruction with path '%s' not found", path.c_str());
    return false;
  }
  std::unique_ptr<Instruction> clone(InstructionHelper::CloneInstruction(instr));
  if (InstructionHelper::InitialiseVariableAttributes(*clone, GetAttributes()))
  {
    log::Debug("Include::SetupImpl(): variable attributes successfully set");
    SetInstruction(clone.release());
    return SetupChild(proc);
  }
  log::Warning(
      "Include::SetupImpl(): instruction with path '%s' could not be "
      "properly initialised with the given attributes",
      path.c_str());
  return false;
}

std::string Include::GetPath() const
{
  return GetAttribute(PATH_ATTRIBUTE_NAME);
}

ExecutionStatus Include::CalculateStatus() const
{
  if (!HasChild())
  {
    return ExecutionStatus::SUCCESS;
  }
  return GetChildStatus();
}

Include::Include() : DecoratorInstruction(Type) {}

Include::~Include() = default;

void Include::SetFilename(const std::string& filename)
{
  _filename = filename;
}

std::string Include::GetFilename() const
{
  return _filename;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
