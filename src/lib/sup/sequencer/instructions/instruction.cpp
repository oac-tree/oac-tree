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

#include <sup/sequencer/instruction.h>

#include "instruction_helper.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_type_parser.h>
#include <sup/dto/json_value_parser.h>

namespace
{
using sup::sequencer::Instruction;
using sup::sequencer::Workspace;
using sup::sequencer::UserInterface;
std::string WrapOptionalInstructionNameString(const std::string& instr_name);
std::string InstructionSetupExceptionMessage(const Instruction& instruction,
                                             const std::vector<std::string>& failed_constraints);
bool GetValueFromVariableName(const Instruction& instruction, const Workspace& ws,
                              UserInterface& ui, const std::string& var_name,
                              sup::dto::AnyValue& value);
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
  AddAttributeDefinition(Constants::NAME_ATTRIBUTE_NAME, sup::dto::StringType);
  AddAttributeDefinition(Constants::IS_ROOT_ATTRIBUTE_NAME, sup::dto::BooleanType);
}

Instruction::~Instruction() = default;

Instruction::Category Instruction::GetCategory() const
{
  return kAction;
}

std::string Instruction::GetType() const
{
  return m_type;
}

std::string Instruction::GetName() const
{
  return GetAttributeString(Constants::NAME_ATTRIBUTE_NAME);
}

void Instruction::SetName(const std::string &name)
{
  if (HasAttribute(Constants::NAME_ATTRIBUTE_NAME))
  {
    SetAttribute(Constants::NAME_ATTRIBUTE_NAME, name);
  }
  else
  {
    AddAttribute(Constants::NAME_ATTRIBUTE_NAME, name);
  }
}

ExecutionStatus Instruction::GetStatus() const
{
  std::lock_guard<std::mutex> lock(m_status_mutex);
  return m_status;
}

void Instruction::Setup(const Procedure& proc)
{
  if (!m_attribute_handler.ValidateAttributes())
  {
    auto error_message =
      InstructionSetupExceptionMessage(*this, m_attribute_handler.GetFailedConstraints());
    throw InstructionSetupException(error_message);
  }
  return SetupImpl(proc);
}

void Instruction::ExecuteSingle(UserInterface& ui, Workspace& ws)
{
  Preamble(ui, ws);
  m_status_before = GetStatus();
  if (NeedsExecute(m_status_before) && !IsHaltRequested())
  {
    SetStatus(ExecuteSingleImpl(ui, ws));
  }
  Postamble(ui);
}

void Instruction::Halt()
{
  m_halt_requested.store(true);
  HaltImpl();
}

void Instruction::Reset(UserInterface& ui)
{
  ResetHook(ui);
  m_status_before = GetStatus();
  SetStatus(ExecutionStatus::NOT_STARTED);
  if (GetStatus() != m_status_before)
  {
    ui.UpdateInstructionStatus(this);
  }
  m_attribute_handler.ClearFailedConstraints();
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

bool Instruction::InitialisePlaceholderAttributes(const StringAttributeList& source_attributes)
{
  bool status =
    InstructionHelper::InitialisePlaceholderAttributes(m_attribute_handler, source_attributes) &&
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

std::vector<Instruction*> Instruction::ChildInstructions()
{
  std::vector<Instruction*> result;
  for (auto instr : static_cast<const Instruction &>(*this).ChildInstructions())
  {
    result.push_back(const_cast<Instruction *>(instr));
  }
  return result;
}

std::vector<const Instruction*> Instruction::ChildInstructions() const
{
  return ChildInstructionsImpl();
}

std::vector<const Instruction*> Instruction::NextInstructions() const
{
  return NextInstructionsImpl();
}

bool Instruction::InsertInstruction(std::unique_ptr<Instruction>&& child, int index)
{
  return InsertInstructionImpl(std::move(child), index);
}

std::unique_ptr<Instruction> Instruction::TakeInstruction(int index)
{
  return TakeInstructionImpl(index);
}

bool Instruction::IsHaltRequested() const
{
  return m_halt_requested.load();
}

AttributeDefinition& Instruction::AddAttributeDefinition(const std::string& attr_name,
                                                         const sup::dto::AnyType& value_type)
{
  return m_attribute_handler.AddAttributeDefinition(attr_name, value_type);
}

AttributeDefinition& Instruction::AddAttributeDefinition(const std::string& attr_name)
{
  return AddAttributeDefinition(attr_name, sup::dto::StringType);
}

bool Instruction::GetAttributeValue(const std::string& attr_name, const Workspace& ws,
                                    UserInterface& ui, sup::dto::AnyValue& value) const
{
  if (!HasAttribute(attr_name))
  {
    // If this attribute was mandatory, Instruction::Setup would have thrown.
    return true;
  }
  auto val_info = GetAttributeValueInfo(m_attribute_handler.GetStringAttributes(),
                                        m_attribute_handler.GetAttributeDefinitions(), attr_name);
  if (val_info.m_is_varname)
  {
    return GetValueFromVariableName(*this, ws, ui, val_info.m_value, value);
  }
  if (!m_attribute_handler.GetValue(attr_name, value))
  {
    const std::string error = InstructionErrorProlog(*this) +
      "could not retrieve AnyValue of attribute [" + attr_name + "] or assign it to passed output "
      "parameter";
    LogError(ui, error);
    return false;
  }
  return true;
}

void Instruction::AddConstraint(Constraint constraint)
{
  return m_attribute_handler.AddConstraint(constraint);
}

void Instruction::SetStatus(ExecutionStatus status)
{
  std::lock_guard<std::mutex> lock(m_status_mutex);
  m_status = status;
}

void Instruction::Preamble(UserInterface& ui, Workspace& ws)
{
  if (GetStatus() == ExecutionStatus::NOT_STARTED)
  {
    if (!InitHook(ui, ws))
    {
      SetStatus(ExecutionStatus::FAILURE);
    }
    else
    {
      SetStatus(ExecutionStatus::NOT_FINISHED);
    }
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

bool Instruction::InitHook(UserInterface&, Workspace&)
{
  return true;
}

void Instruction::HaltImpl() {}

void Instruction::ResetHook(UserInterface&) {}

bool Instruction::PostInitialiseVariables(const StringAttributeList&)
{
  return true;
}

int Instruction::ChildrenCountImpl() const
{
  return 0;
}

std::vector<const Instruction*> Instruction::ChildInstructionsImpl() const
{
  return {};
}

std::vector<const Instruction*> Instruction::NextInstructionsImpl() const
{
  return {};
}

bool Instruction::InsertInstructionImpl(std::unique_ptr<Instruction>&&, int)
{
  return false;
}

std::unique_ptr<Instruction> Instruction::TakeInstructionImpl(int)
{
  return nullptr;
}

bool AppendChildInstruction(Instruction& instruction, std::unique_ptr<Instruction>&& child)
{
  auto n_children = instruction.ChildrenCount();
  return instruction.InsertInstruction(std::move(child), n_children);
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

bool SetValueFromAttributeName(const Instruction& instruction, Workspace& ws,
                               UserInterface& ui, const std::string& attr_name,
                               const sup::dto::AnyValue& value)
{
  auto output_field_name = instruction.GetAttributeString(attr_name);
  if (output_field_name.empty())
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "trying to use variable with empty name";
    LogError(ui, error_message);
    return false;
  }
  auto output_var_name = SplitFieldName(output_field_name).first;
  if (!ws.HasVariable(output_var_name))
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "workspace does not contain output variable with name [" + output_var_name + "]";
    LogError(ui, error_message);
    return false;
  }
  if (!ws.SetValue(output_field_name, value))
  {
    std::string warning_message = InstructionWarningProlog(instruction) +
      "could not write output field with name [" + output_field_name + "] to workspace";
    LogWarning(ui, warning_message);
    return false;
  }
  return true;
}

sup::dto::AnyValue ParseAnyValueAttributePair(const Instruction& instruction,
                                              const Workspace& ws,
                                              UserInterface& ui,
                                              const std::string& type_attr_name,
                                              const std::string& value_attr_name)
{
  auto type_str = instruction.GetAttributeString(type_attr_name);
  sup::dto::JSONAnyTypeParser type_parser;
  const auto& registry = ws.GetTypeRegistry();
  if (!type_parser.ParseString(type_str, std::addressof(registry)))
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "could not parse type [" + type_str + "] from attribute [" + type_attr_name + "]";
    LogError(ui, error_message);
    return {};
  }
  sup::dto::AnyType anytype = type_parser.MoveAnyType();
  auto val_str = instruction.GetAttributeString(value_attr_name);
  sup::dto::JSONAnyValueParser val_parser;
  if (!val_parser.TypedParseString(anytype, val_str))
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "could not parse value [" + val_str + "] from attribute [" + value_attr_name +
      "] to type [" + type_str + "]";
    LogError(ui, error_message);
    return {};
  }
  return val_parser.MoveAnyValue();
}

std::vector<std::string> GetDefinedAttributeNames(const Instruction& instruction)
{
  std::vector<std::string> result;
  for (const auto& attr_def : instruction.GetAttributeDefinitions())
  {
    result.push_back(attr_def.GetName());
  }
  return result;
}

bool IsDefinedAttributeName(const Instruction& instruction, const std::string& attr_name)
{
  auto defined_attr_names = GetDefinedAttributeNames(instruction);
  auto it = std::find(defined_attr_names.begin(), defined_attr_names.end(), attr_name);
  return it != defined_attr_names.end();
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

std::string InstructionSetupExceptionMessage(const sup::sequencer::Instruction& instruction,
                                             const std::vector<std::string>& failed_constraints)
{
  std::string message =
    InstructionSetupExceptionProlog(instruction) + "Failed attribute constraint(s):" +
    sup::sequencer::FormatFailedConstraints(failed_constraints);
  return message;
}

bool GetValueFromVariableName(const Instruction& instruction, const Workspace& ws,
                              UserInterface& ui, const std::string& var_name,
                              sup::dto::AnyValue& value)
{
  if (var_name.empty())
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "trying to fetch variable with empty name";
    LogError(ui, error_message);
    return false;
  }
  auto input_var_name = sup::sequencer::SplitFieldName(var_name).first;
  if (!ws.HasVariable(input_var_name))
  {
    std::string error_message = InstructionErrorProlog(instruction) +
      "workspace does not contain input variable with name [" + input_var_name + "]";
    LogError(ui, error_message);
    return false;
  }
  sup::dto::AnyValue tmp_val;
  if (!ws.GetValue(var_name, tmp_val))
  {
    std::string warning_message = InstructionWarningProlog(instruction) +
      "could not read input field with name [" + var_name + "] from workspace";
    LogWarning(ui, warning_message);
    return false;
  }
  if (!sup::dto::TryAssign(value, tmp_val))
  {
    std::string warning_message = InstructionErrorProlog(instruction) +
      "could not asssign value of field with name [" + var_name + "] to passed output parameter";
    LogError(ui, warning_message);
    return false;
  }
  return true;
}

}  // unnamed namespace
