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

#include <sup/sequencer/instruction.h>

#include "instruction_helper.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

namespace
{
std::string WrapOptionalInstructionNameString(const std::string& instr_name);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{

Instruction::Instruction(const std::string &type)
    : m_type{type}
    , m_status{ExecutionStatus::NOT_STARTED}
    , m_status_before{ExecutionStatus::NOT_STARTED}
    , m_halt_requested{false}
    , m_attribute_handler{}
    , m_status_mutex{}
{
  AddAttributeDefinition(attributes::kNameAttribute, sup::dto::StringType);
  AddAttributeDefinition(kIsRootAttribute, sup::dto::BooleanType);
}

Instruction::~Instruction() = default;

std::string Instruction::GetType() const
{
  return m_type;
}

std::string Instruction::GetName() const
{
  return GetAttributeString(attributes::kNameAttribute);
}

void Instruction::SetName(const std::string &name)
{
  if (HasAttribute(attributes::kNameAttribute))
  {
    SetAttribute(attributes::kNameAttribute, name);
  }
  else
  {
    AddAttribute(attributes::kNameAttribute, name);
  }
}

ExecutionStatus Instruction::GetStatus() const
{
  std::lock_guard<std::mutex> lock(m_status_mutex);
  return m_status;
}

void Instruction::Setup(const Procedure& proc)
{
  if (!m_attribute_handler.InitValueMap())
  {
    auto error_message = "Oops";
      // TODO: create correct exception message
      // VariableSetupExceptionMessage(*this, m_attribute_handler.GetFailedConstraints());
    throw InstructionSetupException(error_message);
  }
  return SetupImpl(proc);
}

void Instruction::ExecuteSingle(UserInterface& ui, Workspace& ws)
{
  Preamble(ui);
  m_status_before = GetStatus();
  SetStatus(ExecuteSingleImpl(ui, ws));
  Postamble(ui);
}

void Instruction::Halt()
{
  m_halt_requested.store(true);
  HaltImpl();
}

void Instruction::Reset()
{
  ResetHook();
  SetStatus(ExecutionStatus::NOT_STARTED);
  m_halt_requested.store(false);
}

bool Instruction::HasAttribute(const std::string& name) const
{
  return m_attribute_handler.HasStringAttribute(name);
}

std::string Instruction::GetAttributeString(const std::string& name) const
{
  return GetStringAttributeValue(m_attribute_handler.GetStringAttributes(), name);
}

const StringAttributeList& Instruction::GetStringAttributes() const
{
  return m_attribute_handler.GetStringAttributes();
}

bool Instruction::AddAttribute(const std::string& name, const std::string& value)
{
  return m_attribute_handler.AddStringAttribute(name, value);
}

bool Instruction::SetAttribute(const std::string& name, const std::string& value)
{
  if (!HasAttribute(name))
  {
    return false;
  }
  m_attribute_handler.SetStringAttribute(name, value);
  return true;
}

bool Instruction::AddAttributes(const StringAttributeList& attributes)
{
  bool result = true;
  for (const auto &attr : attributes)
  {
    result = AddAttribute(attr.first, attr.second) && result;
  }
  return result;
}

bool Instruction::InitialiseVariableAttributes(const StringAttributeList& source_attributes)
{
  bool status =
    InstructionHelper::InitialiseVariableAttributes(m_attribute_handler, source_attributes) &&
    PostInitialiseVariables(source_attributes);
  return status;
}

const std::vector<AttributeDefinition>& Instruction::GetAttributeDefinitions() const
{
  return m_attribute_handler.GetAttributeDefinitions();
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

AttributeDefinition& Instruction::AddAttributeDefinition(const std::string& attr_name,
                                                      const sup::dto::AnyType& value_type)
{
  return m_attribute_handler.AddAttributeDefinition(attr_name, value_type);
}

void Instruction::AddConstraint(Constraint constraint)
{
  return m_attribute_handler.AddConstraint(constraint);
}

bool Instruction::IsHaltRequested() const
{
  return m_halt_requested.load();
}

void Instruction::SetStatus(ExecutionStatus status)
{
  std::lock_guard<std::mutex> lock(m_status_mutex);
  m_status = status;
}

void Instruction::Preamble(UserInterface& ui)
{
  if (GetStatus() == ExecutionStatus::NOT_STARTED)
  {
    InitHook();
    SetStatus(ExecutionStatus::NOT_FINISHED);
    ui.UpdateInstructionStatus(this);
  }
}

void Instruction::Postamble(UserInterface& ui)
{
  if (GetStatus() != m_status_before)
  {
    ui.UpdateInstructionStatus(this);
  }
}

void Instruction::SetupImpl(const Procedure&) {}

void Instruction::InitHook() {}

void Instruction::HaltImpl() {}

void Instruction::ResetHook() {}

bool Instruction::PostInitialiseVariables(const StringAttributeList&)
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

std::string InstructionSetupExceptionProlog(const Instruction& instruction)
{
  auto instr_name = instruction.GetName();
  auto instr_type = instruction.GetType();
  std::string optional_name = WrapOptionalInstructionNameString(instr_name);
  return "Setup of instruction " + optional_name + "of type <" + instr_type + "> failed: ";
}

std::string InstructionErrorProlog(const Instruction& instruction)
{
  auto instr_name = instruction.GetName();
  auto instr_type = instruction.GetType();
  std::string optional_name = WrapOptionalInstructionNameString(instr_name);
  return "Instruction " + optional_name + "of type <" + instr_type + "> error: ";
}

std::string InstructionWarningProlog(const Instruction& instruction)
{
  auto instr_name = instruction.GetName();
  auto instr_type = instruction.GetType();
  std::string optional_name = WrapOptionalInstructionNameString(instr_name);
  return "Instruction " + optional_name + "of type <" + instr_type + "> warning: ";
}

bool GetValueFromAttributeName(const Instruction& instruction, const Workspace& ws,
                               UserInterface& ui, const std::string& attr_name,
                               sup::dto::AnyValue& value)
{
  auto input_field_name = instruction.GetAttributeValue<std::string>(attr_name);
  if (input_field_name.empty())
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "trying to fetch variable with empty name";
    ui.LogError(error_message);
    return false;
  }
  auto input_var_name = SplitFieldName(input_field_name).first;
  if (!ws.HasVariable(input_var_name))
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "workspace does not contain input variable with name [" + input_var_name + "]";
    ui.LogError(error_message);
    return false;
  }
  if (!ws.GetValue(input_field_name, value))
  {
    std::string warning_message = InstructionErrorProlog(instruction) +
      "could not read input field with name [" + input_field_name + "] from workspace";
    ui.LogWarning(warning_message);
    return false;
  }
  return true;
}

bool SetValueFromAttributeName(const Instruction& instruction, Workspace& ws,
                               UserInterface& ui, const std::string& attr_name,
                               const sup::dto::AnyValue& value)
{
  auto output_field_name = instruction.GetAttributeValue<std::string>(attr_name);
  if (output_field_name.empty())
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "trying to use variable with empty name";
    ui.LogError(error_message);
    return false;
  }
  auto output_var_name = SplitFieldName(output_field_name).first;
  if (!ws.HasVariable(output_var_name))
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "workspace does not contain output variable with name [" + output_var_name + "]";
    ui.LogError(error_message);
    return false;
  }
  if (!ws.SetValue(output_field_name, value))
  {
    std::string warning_message = InstructionErrorProlog(instruction) +
      "could not write output field with name [" + output_field_name + "] to workspace";
    ui.LogWarning(warning_message);
    return false;
  }
  return true;
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
