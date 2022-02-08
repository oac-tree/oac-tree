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
#include <common/AnyValue.h>

#include <common/log-api.h>

// Local header files

#include "Procedure.h"
#include "Sequence.h"
#include "SequenceParser.h"
#include "Workspace.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace sup
{
namespace sequencer
{
const std::string Procedure::TICK_TIMEOUT_ATTRIBUTE_NAME = "tickTimeout";

// Type definition

// Global variables

static const std::string IS_ROOT_ATTRIBUTE = "isRoot";

// Function declaration

static bool HasRootAttributeSet(const Instruction &instruction);

// Function definition

const Procedure *Procedure::LoadProcedure(const std::string &filename) const
{
  if (_procedure_cache.find(filename) == _procedure_cache.end())
  {
    log_debug("Procedure::LoadProcedure('%s') - Loading procedure from file..", filename.c_str());
    auto loaded_proc = ParseProcedureFile(filename);
    if (loaded_proc)
    {
      _procedure_cache[filename] = std::move(loaded_proc);
    }
  }
  if (_procedure_cache.find(filename) == _procedure_cache.end())
  {
    log_warning("Procedure::LoadProcedure('%s') - Could not load procedure from file..",
                filename.c_str());
    return nullptr;
  }
  return _procedure_cache[filename].get();
}

void Procedure::ClearProcedureCache() const
{
  _procedure_cache.clear();
}

Procedure::Procedure() : _workspace{new Workspace()} {}

Procedure::~Procedure()
{
  // Wait for termination of instructions first:
  Reset();
}

void Procedure::SetFilename(const std::string &filename)
{
  _filename = filename;
}

std::string Procedure::GetFilename() const
{
  return _filename;
}

Instruction *Procedure::RootInstruction()
{
  // Scott Meyers' solution for avoiding code duplication:
  return const_cast<Instruction *>(static_cast<const Procedure &>(*this).RootInstruction());
}

const Instruction *Procedure::RootInstruction() const
{
  if (_instructions.empty())
  {
    return nullptr;
  }
  if (_instructions.size() == 1)
  {
    return _instructions[0].get();
  }
  for (auto &instr : _instructions)
  {
    if (HasRootAttributeSet(*instr))
    {
      return instr.get();
    }
  }
  return nullptr;
}

std::vector<const Instruction *> Procedure::GetInstructions(const std::string &filename) const
{
  std::vector<const Instruction *> result;
  if (filename.empty() || filename == GetFilename())
  {
    for (auto &instr : _instructions)
    {
      result.push_back(instr.get());
    }
  }
  else
  {
    auto loaded_proc = LoadProcedure(filename);
    if (loaded_proc)
    {
      return loaded_proc->GetInstructions();
    }
    log_warning("Procedure::GetInstructions('%s') - finding instructions failed..",
                filename.c_str());
  }
  return result;
}

int Procedure::GetInstructionCount() const
{
  return static_cast<int>(_instructions.size());
}

bool Procedure::PushInstruction(Instruction *instruction)
{
  if (instruction == nullptr)
  {
    log_warning("Procedure::PushInstruction() - trying to add null pointer");
    return false;
  }
  _instructions.emplace_back(instruction);
  return true;
}

bool Procedure::InsertInstruction(Instruction *instruction, int index)
{
  if (index < 0 || index > _instructions.size())
    return false;
  _instructions.emplace(std::next(_instructions.begin(), index), instruction);
  return true;
}

Instruction *Procedure::TakeInstruction(int index)
{
  if (index < 0 || index >= _instructions.size())
    return nullptr;

  auto it = std::next(_instructions.begin(), index);
  auto retval = std::move(*it);
  _instructions.erase(it);
  return retval.release();
}

bool Procedure::AddVariable(std::string name, Variable *var)
{
  return _workspace->AddVariable(name, var);
}

std::vector<std::string> Procedure::VariableNames() const
{
  return _workspace->VariableNames();
}

bool Procedure::GetVariableValue(std::string name, ::ccs::types::AnyValue &value) const
{
  return _workspace->GetValue(name, value);
}

bool Procedure::Setup()
{
  if (RootInstruction() == nullptr)
  {
    return true;
  }
  ClearProcedureCache();
  return RootInstruction()->Setup(*this);
}

void Procedure::ExecuteSingle(UserInterface *ui)
{
  if (RootInstruction() == nullptr)
  {
    return;
  }
  RootInstruction()->ExecuteSingle(ui, _workspace.get());
}

void Procedure::Halt()
{
  if (RootInstruction() == nullptr)
  {
    return;
  }
  RootInstruction()->Halt();
}

void Procedure::Reset()
{
  if (RootInstruction() == nullptr)
  {
    return;
  }
  RootInstruction()->Reset();
}

ExecutionStatus Procedure::GetStatus() const
{
  if (RootInstruction() == nullptr)
  {
    return ExecutionStatus::SUCCESS;
  }
  return RootInstruction()->GetStatus();
}

bool Procedure::HasAttribute(const std::string &name) const
{
  return _attributes.HasAttribute(name);
}

std::string Procedure::GetAttribute(const std::string &name) const
{
  return _attributes.GetAttribute(name);
}

bool Procedure::AddAttribute(const std::string &name, const std::string &value)
{
  return _attributes.AddAttribute(name, value);
}

bool Procedure::AddAttributes(const AttributeMap &attributes)
{
  bool result = true;
  for (auto &attr : attributes)
  {
    result = AddAttribute(attr.first, attr.second) && result;
  }
  return result;
}

const Workspace *Procedure::GetWorkspace() const
{
  return _workspace.get();
}

bool Procedure::RegisterGenericCallback(
    const std::function<void(const std::string&, const ccs::types::AnyValue&)>& cb)
{
  return _workspace->RegisterGenericCallback(cb);
}

static bool HasRootAttributeSet(const Instruction &instruction)
{
  if (!instruction.HasAttribute(IS_ROOT_ATTRIBUTE))
  {
    return false;
  }
  auto attr_val = instruction.GetAttribute(IS_ROOT_ATTRIBUTE);
  return attributes::AttributeAsBool(attr_val);
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
