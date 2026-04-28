/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/instruction.h>

#include "instruction_helper.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/json_type_parser.h>
#include <sup/dto/json_value_parser.h>

namespace
{
using sup::oac_tree::ExecutionStatus;
using sup::oac_tree::Instruction;
using sup::oac_tree::Workspace;
using sup::oac_tree::UserInterface;
ExecutionStatus NotificationStatus(bool halt_requested, const ExecutionStatus& status);
bool DoNotUpdateStatus(const ExecutionStatus& old_status, const ExecutionStatus& new_status,
                       bool halt_requested);
std::string WrapOptionalInstructionNameString(const std::string& instr_name);
std::string InstructionSetupExceptionMessage(const Instruction& instruction,
                                             const std::vector<std::string>& failed_constraints);
bool GetValueFromVariableName(const Instruction& instruction, const Workspace& ws,
                              UserInterface& ui, const std::string& var_name,
                              sup::dto::AnyValue& value);
}  // unnamed namespace

namespace sup
{
namespace oac_tree
{

Instruction::Instruction(const std::string &type)
    : m_type{type}
    , m_status{ExecutionStatus::NOT_STARTED}
    , m_halt_requested{false}
    , m_attribute_handler{}
    , m_status_mutex{}
    , m_breakpoint_state{Breakpoint::kUnset}
{
  (void)AddAttributeDefinition(Constants::NAME_ATTRIBUTE_NAME, sup::dto::StringType);
  (void)AddAttributeDefinition(Constants::IS_ROOT_ATTRIBUTE_NAME, sup::dto::BooleanType);
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
    (void)SetAttribute(Constants::NAME_ATTRIBUTE_NAME, name);
  }
  else
  {
    (void)AddAttribute(Constants::NAME_ATTRIBUTE_NAME, name);
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
  if (CheckBreakpoint())
  {
    return;
  }
  Preamble(ui, ws);
  if (NeedsExecute(GetStatus()) && !IsHaltRequested())
  {
    SetStatus(ExecuteSingleImpl(ui, ws), ui);
  }
}

void Instruction::Halt(UserInterface& ui)
{
  bool status_updated{false};
  ExecutionStatus status{ExecutionStatus::NOT_STARTED};
  {
    std::lock_guard<std::mutex> lk{m_status_mutex};
    status_updated = (NotificationStatus(true, m_status)
                      != NotificationStatus(m_halt_requested, m_status));
    m_halt_requested = true;
    status = m_status;
  }
  if (status_updated)
  {
    ui.UpdateInstructionStatus(this, NotificationStatus(true, status));
  }
  HaltImpl(ui);
}

void Instruction::Reset(UserInterface& ui)
{
  bool status_updated{false};
  ResetHook(ui);
  {
    std::lock_guard<std::mutex> lk{m_status_mutex};
    status_updated = (m_status != ExecutionStatus::NOT_STARTED);
    m_halt_requested = false;
    m_status = ExecutionStatus::NOT_STARTED;
  }
  if (status_updated)
  {
    ui.UpdateInstructionStatus(this, ExecutionStatus::NOT_STARTED);
  }
  m_attribute_handler.ClearFailedConstraints();
  ResetBreakpoint();
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
  for (const auto& [attr_name, attr_value] : attributes)
  {
    result = AddAttribute(attr_name, attr_value) && result;
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
  std::lock_guard<std::mutex> lk{m_status_mutex};
  return m_halt_requested;
}

void Instruction::SetBreakpoint() const
{
  auto expected_state = Breakpoint::kUnset;
  (void)m_breakpoint_state.compare_exchange_strong(expected_state, Breakpoint::kSet);
}

void Instruction::RemoveBreakpoint() const
{
  m_breakpoint_state.store(Breakpoint::kUnset);
}

bool Instruction::AcknowledgeBreakpoint() const
{
  auto expected_state = Breakpoint::kHit;
  return m_breakpoint_state.compare_exchange_strong(expected_state, Breakpoint::kAck);
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

void Instruction::SetStatus(ExecutionStatus status, UserInterface& ui)
{
  bool halt_requested{false};
  {
    std::lock_guard<std::mutex> lock(m_status_mutex);
    halt_requested = m_halt_requested;
    if (DoNotUpdateStatus(m_status, status, halt_requested))
    {
      return;
    }
    m_status = status;
  }
  ui.UpdateInstructionStatus(this, NotificationStatus(halt_requested, status));
}

void Instruction::Preamble(UserInterface& ui, Workspace& ws)
{
  if (GetStatus() == ExecutionStatus::NOT_STARTED)
  {
    if (!InitHook(ui, ws))
    {
      SetStatus(ExecutionStatus::FAILURE, ui);
    }
    else
    {
      SetStatus(ExecutionStatus::NOT_FINISHED, ui);
    }
  }
}

bool Instruction::CheckBreakpoint()
{
  auto expected_state = Breakpoint::kSet;
  if (m_breakpoint_state.compare_exchange_strong(expected_state, Breakpoint::kHit)
      || expected_state == Breakpoint::kHit)
  {
    return true;
  }
  expected_state = Breakpoint::kAck;
  (void)m_breakpoint_state.compare_exchange_strong(expected_state, Breakpoint::kSet);
  return false;
}

void Instruction::ResetBreakpoint()
{
  if (m_breakpoint_state.load() == Breakpoint::kUnset)
  {
    return;
  }
  m_breakpoint_state.store(Breakpoint::kSet);
}

void Instruction::SetupImpl(const Procedure&) {}

bool Instruction::InitHook(UserInterface&, Workspace&)
{
  return true;
}

void Instruction::HaltImpl(UserInterface&) {}

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

}  // namespace oac_tree

}  // namespace sup

namespace
{
ExecutionStatus NotificationStatus(bool halt_requested, const ExecutionStatus& status)
{
  if (halt_requested && IsExecutingStatus(status))
  {
    return ExecutionStatus::HALTED;
  }
  return status;
}

bool DoNotUpdateStatus(const ExecutionStatus& old_status, const ExecutionStatus& new_status,
                       bool halt_requested)
{
  const auto same_status = (new_status == old_status);
  const auto finished_when_halted = halt_requested && IsFinishedStatus(new_status);
  return same_status || finished_when_halted;
}

std::string WrapOptionalInstructionNameString(const std::string& instr_name)
{
  if (instr_name.empty())
  {
    return {};
  }
  return "[" + instr_name + "] ";
}

std::string InstructionSetupExceptionMessage(const sup::oac_tree::Instruction& instruction,
                                             const std::vector<std::string>& failed_constraints)
{
  std::string message =
    InstructionSetupExceptionProlog(instruction) + "Failed attribute constraint(s):" +
    sup::oac_tree::FormatFailedConstraints(failed_constraints);
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
  auto input_var_name = sup::oac_tree::SplitFieldName(var_name).first;
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
