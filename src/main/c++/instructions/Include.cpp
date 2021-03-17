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

static std::string GetFullPathName(const std::string & directory, const std::string & filename);
static std::string GetFileDirectory(const std::string & filename);

// Function definition

ExecutionStatus Include::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
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

bool Include::SetupImpl(const Procedure & proc)
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
    log_warning("Include::SetupImpl(): instruction with path '%s' not found",
                path.c_str());
    return false;
  }
  std::unique_ptr<Instruction> clone(InstructionHelper::CloneInstruction(instr));
  if (InstructionHelper::InitialiseVariableAttributes(*clone, GetAttributes()))
  {
    log_info("Include::SetupImpl(): variable attributes successfully set");
    SetInstruction(clone.release());
    return SetupChild(proc);
  }
  log_warning("Include::SetupImpl(): instruction with path '%s' could not be "
              "properly initialised with the given attributes", path.c_str());
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

Include::Include()
  : DecoratorInstruction(Type)
{}

Include::~Include()
{}

void Include::SetFilename(const std::string & filename)
{
  _filename = filename;
}

std::string Include::GetFilename() const
{
  return _filename;
}

static std::string GetFullPathName(const std::string & directory, const std::string & filename)
{
  log_info("GetFullPathName(%s, %s) - entering function..",
           directory.c_str(), filename.c_str());
  if (filename.empty())
  {
    log_warning("GetFullPathName() - empty filename as argument");
    return {};
  }
  if (filename.front() == '/')
  {
    return filename;
  }
  return directory + filename;
}

static std::string GetFileDirectory(const std::string & filename)
{
  auto pos = filename.find_last_of("/");
  if (pos == std::string::npos)
  {
    return {};
  }
  return filename.substr(0, pos + 1);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
