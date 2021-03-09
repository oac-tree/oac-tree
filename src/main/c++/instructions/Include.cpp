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
* Copyright (c) : 2010-2020 ITER Organization,
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

#include <common/log-api.h>

// Local header files

#include "Include.h"
#include "InstructionHelper.h"
#include "Procedure.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

static const std::string PATH_ATTRIBUTE_NAME="path";
static const std::string FILE_ATTRIBUTE_NAME="file";

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string Include::Type = "Include";

// Function declaration

static std::string GetFullPathname(const std::string & filename,
                                   const std::string & current_directory);

// Function definition

ExecutionStatus Include::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  auto child_status = _child->GetStatus();
  if (NeedsExecute(child_status))
  {
    _child->ExecuteSingle(ui, ws);
  }

  return CalculateStatus();
}

bool Include::PostInitialiseVariables(const AttributeMap & source)
{
  bool result = true;
  for (auto & attr : source)
  {
    if (!HasAttribute(attr.first))
    {
      result = AddAttribute(attr.first, attr.second) && result;
    }
  }
  return result;
}

std::string Include::GetPath() const
{
  return GetAttribute(PATH_ATTRIBUTE_NAME);
}

ExecutionStatus Include::CalculateStatus() const
{
  if (!_child)
  {
    return ExecutionStatus::SUCCESS;
  }

  return _child->GetStatus();
}

bool Include::SetupImpl(const Procedure & proc)
{
  std::string full_filename;
  if (HasAttribute(FILE_ATTRIBUTE_NAME))
  {
    auto filename = GetAttribute(FILE_ATTRIBUTE_NAME);
    full_filename = GetFullPathname(filename, _current_directory);
  }
  auto instructions = proc.GetInstructions(full_filename);
  auto path = GetPath();
  auto instr = InstructionHelper::FindInstruction(instructions, path);
  if (instr == nullptr)
  {
    log_warning("Include::SetupImpl(): instruction with path '%s' not found",
                path.c_str());
    return false;
  }
  std::unique_ptr<Instruction> clone(InstructionHelper::CloneInstruction(instr));
  if (InstructionHelper::InitialiseVariableAttributes(*clone, GetAttributes()))
  {
    log_info("Include::SetupImpl(): variable attributes successfully set");
    SetInstruction(clone.release());
    return _child->Setup(proc);
  }
  log_warning("Include::SetupImpl(): instruction with path '%s' could not be "
              "properly initialised with the given attributes", path.c_str());
  return false;
}

Include::Include()
  : DecoratorInstruction(Type)
{}

Include::~Include()
{}

void Include::SetCurrentDirectory(const std::string & directory)
{
  _current_directory = directory;
}

std::string Include::GetCurrentDirectory() const
{
  return _current_directory;
}

static std::string GetFullPathname(const std::string & filename,
                                   const std::string & current_directory)
{
  log_info("GetFullPathname(%s, %s) - entering function..",
           filename.c_str(), current_directory.c_str());
  if (filename.empty())
  {
    log_warning("GetFullPathname() - empty filename as argument");
    return {};
  }
  if (filename.front() == '/')
  {
    return filename;
  }
  return current_directory + filename;
}



} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
