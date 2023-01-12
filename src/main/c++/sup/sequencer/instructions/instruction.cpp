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

#include <sup/sequencer/instruction.h>

#include <sup/sequencer/user_interface.h>

namespace
{
std::string WrapOptionalInstructionNameString(const std::string& instr_name);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{


Instruction::Instruction(const std::string &type)
    : _type{type}
    , _status{ExecutionStatus::NOT_STARTED}
    , _status_before{ExecutionStatus::NOT_STARTED}
    , _halt_requested{false}
{}

Instruction::~Instruction() = default;

std::string Instruction::GetType() const
{
  return _type;
}

std::string Instruction::GetName() const
{
  return GetAttribute(attributes::NAME_ATTRIBUTE);
}

void Instruction::SetName(const std::string &name)
{
  if (HasAttribute(attributes::NAME_ATTRIBUTE))
    SetAttribute(attributes::NAME_ATTRIBUTE, name);
  else
    AddAttribute(attributes::NAME_ATTRIBUTE, name);
}

ExecutionStatus Instruction::GetStatus() const
{
  std::lock_guard<std::mutex> lock(_status_mutex);
  return _status;
}

void Instruction::Setup(const Procedure& proc)
{
  return SetupImpl(proc);
}

void Instruction::ExecuteSingle(UserInterface* ui, Workspace* ws)
{
  Preamble(ui);
  _status_before = GetStatus();
  SetStatus(ExecuteSingleImpl(ui, ws));
  Postamble(ui);
}

void Instruction::Halt()
{
  _halt_requested.store(true);
  HaltImpl();
}

void Instruction::Reset()
{
  ResetHook();
  SetStatus(ExecutionStatus::NOT_STARTED);
  _halt_requested.store(false);
}

bool Instruction::HasAttribute(const std::string& name) const
{
  return _attributes.HasAttribute(name);
}

std::string Instruction::GetAttribute(const std::string& name) const
{
  return _attributes.GetAttribute(name);
}

AttributeMap Instruction::GetAttributes() const
{
  return _attributes;
}

bool Instruction::AddAttribute(const std::string& name, const std::string& value)
{
  return _attributes.AddAttribute(name, value);
}

bool Instruction::SetAttribute(const std::string& name, const std::string& value)
{
  if (!_attributes.HasAttribute(name))
    return false;
  _attributes.SetAttribute(name, value);
  return true;
}

bool Instruction::AddAttributes(const AttributeMap& attributes)
{
  bool result = true;
  for (const auto &attr : attributes)
  {
    result = AddAttribute(attr.first, attr.second) && result;
  }
  return result;
}

bool Instruction::InitialiseVariableAttributes(const AttributeMap& source)
{
  bool status = _attributes.InitialiseVariableAttributes(source) && PostInitialiseVariables(source);
  return status;
}

int Instruction::ChildrenCount() const
{
  return ChildrenCountImpl();
}

std::vector<Instruction *> Instruction::ChildInstructions()
{
  std::vector<Instruction *> result;
  for (auto instr : static_cast<const Instruction &>(*this).ChildInstructions())
  {
    result.push_back(const_cast<Instruction *>(instr));
  }
  return result;
}

std::vector<const Instruction *> Instruction::ChildInstructions() const
{
  return ChildInstructionsImpl();
}

bool Instruction::InsertInstruction(Instruction* child, int index)
{
  return InsertInstructionImpl(child, index);
}

Instruction* Instruction::TakeInstruction(int index)
{
  return TakeInstructionImpl(index);
}

std::string Instruction::InstructionSetupExceptionProlog() const
{
  std::string optional_name = WrapOptionalInstructionNameString(GetName());
  return "Setup of instruction " + optional_name + "of type <" + GetType() + "> failed: ";
}

std::string Instruction::InstructionErrorLogProlog() const
{
  std::string optional_name = WrapOptionalInstructionNameString(GetName());
  return "Instruction " + optional_name + "of type <" + GetType() + "> error: ";
}

std::string Instruction::InstructionWarningLogProlog() const
{
  std::string optional_name = WrapOptionalInstructionNameString(GetName());
  return "Instruction " + optional_name + "of type <" + GetType() + "> warning: ";
}

void Instruction::SetStatus(ExecutionStatus status)
{
  std::lock_guard<std::mutex> lock(_status_mutex);
  _status = status;
}

void Instruction::Preamble(UserInterface* ui)
{
  PreExecuteHook(ui);
  if (GetStatus() == ExecutionStatus::NOT_STARTED)
  {
    InitHook();
    SetStatus(ExecutionStatus::NOT_FINISHED);
    ui->UpdateInstructionStatus(this);
  }
}

void Instruction::Postamble(UserInterface* ui)
{
  if (GetStatus() != _status_before)
  {
    ui->UpdateInstructionStatus(this);
  }
  PostExecuteHook(ui);
}

void Instruction::SetupImpl(const Procedure&) {}

void Instruction::PreExecuteHook(UserInterface*) {}

void Instruction::InitHook() {}

void Instruction::PostExecuteHook(UserInterface*) {}

void Instruction::HaltImpl() {}

void Instruction::ResetHook() {}

bool Instruction::PostInitialiseVariables(const AttributeMap&)
{
  return true;
}

int Instruction::ChildrenCountImpl() const
{
  return 0;
}

std::vector<const Instruction *> Instruction::ChildInstructionsImpl() const
{
  return {};
}

bool Instruction::InsertInstructionImpl(Instruction*, int)
{
  return false;
}

Instruction* Instruction::TakeInstructionImpl(int)
{
  return nullptr;
}

}  // namespace sequencer

}  // namespace sup

namespace
{
std::string WrapOptionalInstructionNameString(const std::string& instr_name)
{
  if (instr_name.empty())
  {
    return {};
  }
  return "[" + instr_name + "] ";
}
}  // unnamed namespace
