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

#include <sup/sequencer/procedure.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace sequencer
{
const std::string Procedure::TICK_TIMEOUT_ATTRIBUTE_NAME = "tickTimeout";

static const std::string IS_ROOT_ATTRIBUTE = "isRoot";

static bool HasRootAttributeSet(const Instruction &instruction);

const Procedure *Procedure::LoadProcedure(const std::string &filename) const
{
  if (m_procedure_cache.find(filename) == m_procedure_cache.end())
  {
    m_procedure_cache[filename] = ParseProcedureFile(filename);
  }
  return m_procedure_cache[filename].get();
}

void Procedure::ClearProcedureCache() const
{
  m_procedure_cache.clear();
}

Procedure::Procedure()
  : m_instructions{}
  , m_workspace{new Workspace()}
  , m_attributes{}
  , m_filename{}
  , m_procedure_cache{}
{}

Procedure::~Procedure()
{
  // Wait for termination of instructions first:
  Reset();
}

void Procedure::SetFilename(const std::string &filename)
{
  m_filename = filename;
}

std::string Procedure::GetFilename() const
{
  return m_filename;
}

Instruction *Procedure::RootInstruction()
{
  // Scott Meyers' solution for avoiding code duplication:
  return const_cast<Instruction *>(static_cast<const Procedure &>(*this).RootInstruction());
}

const Instruction *Procedure::RootInstruction() const
{
  if (m_instructions.empty())
  {
    return nullptr;
  }
  if (m_instructions.size() == 1)
  {
    return m_instructions[0].get();
  }
  for (auto &instr : m_instructions)
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
  if (filename.empty() || filename == GetFilename())
  {
    std::vector<const Instruction *> result;
    for (auto &instr : m_instructions)
    {
      result.push_back(instr.get());
    }
    return result;
  }
  auto loaded_proc = LoadProcedure(filename);
  if (!loaded_proc)
  {
    std::string error_message =
      "sup::sequencer::Procedure::GetInstructions: could not load procedure with filename [" +
      filename + "]";
    throw ParseException(error_message);
  }
  return loaded_proc->GetInstructions();
}

int Procedure::GetInstructionCount() const
{
  return static_cast<int>(m_instructions.size());
}

void Procedure::PushInstruction(Instruction *instruction)
{
  if (instruction == nullptr)
  {
    std::string error_message =
      "sup::sequencer::Procedure::PushInstruction(): trying to add nullptr";
    throw InvalidOperationException(error_message);
  }
  m_instructions.emplace_back(instruction);
}

bool Procedure::InsertInstruction(Instruction *instruction, int index)
{
  if (index < 0 || index > m_instructions.size())
    return false;
  m_instructions.emplace(std::next(m_instructions.begin(), index), instruction);
  return true;
}

Instruction *Procedure::TakeInstruction(int index)
{
  if (index < 0 || index >= m_instructions.size())
    return nullptr;

  auto it = std::next(m_instructions.begin(), index);
  auto retval = std::move(*it);
  m_instructions.erase(it);
  return retval.release();
}

bool Procedure::AddVariable(std::string name, Variable *var)
{
  return m_workspace->AddVariable(name, var);
}

std::vector<std::string> Procedure::VariableNames() const
{
  return m_workspace->VariableNames();
}

bool Procedure::GetVariableValue(std::string name, sup::dto::AnyValue &value) const
{
  return m_workspace->GetValue(name, value);
}

bool Procedure::Setup()
{
  m_workspace->Setup();
  if (RootInstruction() == nullptr)
  {
    return true;
  }
  ClearProcedureCache();
  RootInstruction()->Setup(*this);
  return true;
}

void Procedure::ExecuteSingle(UserInterface *ui)
{
  if (RootInstruction() == nullptr)
  {
    return;
  }
  RootInstruction()->ExecuteSingle(ui, m_workspace.get());
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
  m_workspace->Reset();
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
  return m_attributes.HasAttribute(name);
}

std::string Procedure::GetAttribute(const std::string &name) const
{
  return m_attributes.GetAttribute(name);
}

bool Procedure::AddAttribute(const std::string &name, const std::string &value)
{
  return m_attributes.AddAttribute(name, value);
}

bool Procedure::AddAttributes(const std::vector<Attribute>& attributes)
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
  return m_workspace.get();
}

bool Procedure::RegisterType(const sup::dto::AnyType& anytype)
{
  return m_workspace->RegisterType(anytype);
}

const sup::dto::AnyTypeRegistry* Procedure::GetTypeRegistry() const
{
  return m_workspace->GetTypeRegistry();
}

bool Procedure::RegisterGenericCallback(
    const std::function<void(const std::string&, const sup::dto::AnyValue&)>& cb)
{
  return m_workspace->RegisterGenericCallback(cb);
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
