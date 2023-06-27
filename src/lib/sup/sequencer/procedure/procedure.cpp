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

#include <sup/sequencer/procedure.h>

#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_tree.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace sequencer
{
static bool HasRootAttributeSet(const Instruction &instruction);

Procedure::Procedure()
  : m_instructions{}
  , m_workspace{new Workspace()}
  , m_attribute_handler{}
  , m_filename{}
  , m_parent{}
  , m_procedure_store{this}
{
  m_attribute_handler.AddAttributeDefinition(kTickTimeoutAttributeName, sup::dto::Float64Type);
}

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

std::vector<const Instruction*> Procedure::GetTopInstructions() const
{
  std::vector<const Instruction*> result;
  for (auto &instr : m_instructions)
  {
    result.push_back(instr.get());
  }
  return result;
}

const Procedure& Procedure::GetSubProcedure(const std::string& filename) const
{
  if (filename.empty() || filename == GetFilename())
  {
    return *this;
  }
  auto& store = GetProcedureStore();
  return store.GetProcedure(filename);
}

InstructionTree Procedure::GetNextInstructionTree() const
{
  auto root = RootInstruction();
  if (root == nullptr || !ReadyForExecute(root->GetStatus()))
  {
    return InstructionTree{nullptr};
  }
  return CreateNextInstructionTree(root);
}

std::vector<const Instruction*> Procedure::GetNextInstructions() const
{
  auto root = RootInstruction();
  if (root == nullptr || !ReadyForExecute(root->GetStatus()))
  {
    return {};
  }
  return FlattenBFS(CreateNextInstructionTree(root));
}

int Procedure::GetInstructionCount() const
{
  return static_cast<int>(m_instructions.size());
}

void Procedure::PushInstruction(std::unique_ptr<Instruction>&& instruction)
{
  if (!instruction)
  {
    std::string error_message =
      "sup::sequencer::Procedure::PushInstruction(): trying to add empty unique_ptr";
    throw InvalidOperationException(error_message);
  }
  m_instructions.emplace_back(std::move(instruction));
}

bool Procedure::InsertInstruction(std::unique_ptr<Instruction>&& instruction, int index)
{
  if (index < 0 || index > m_instructions.size())
  {
    return false;
  }
  m_instructions.emplace(std::next(m_instructions.begin(), index), std::move(instruction));
  return true;
}

std::unique_ptr<Instruction> Procedure::TakeInstruction(int index)
{
  if (index < 0 || index >= m_instructions.size())
  {
    return nullptr;
  }
  auto it = std::next(m_instructions.begin(), index);
  std::unique_ptr<Instruction> retval;
  std::swap(retval, *it);
  m_instructions.erase(it);
  return std::move(retval);
}

bool Procedure::AddVariable(std::string name, std::unique_ptr<Variable>&& var)
{
  return m_workspace->AddVariable(name, std::move(var));
}

std::vector<std::string> Procedure::VariableNames() const
{
  return m_workspace->VariableNames();
}

bool Procedure::GetVariableValue(std::string name, sup::dto::AnyValue& value) const
{
  return m_workspace->GetValue(name, value);
}

bool Procedure::Setup()
{
  if (!m_attribute_handler.ValidateAttributes())
  {
    std::string error_message = "Procedure::Setup(): Failed attribute constraint(s):" +
                                FormatFailedConstraints(m_attribute_handler.GetFailedConstraints());
    throw ProcedureSetupException(error_message);
  }
  m_workspace->Setup();
  if (RootInstruction() == nullptr)
  {
    return true;
  }
  RootInstruction()->Setup(*this);
  return true;
}

void Procedure::ExecuteSingle(UserInterface& ui)
{
  if (RootInstruction() == nullptr)
  {
    return;
  }
  RootInstruction()->ExecuteSingle(ui, *m_workspace);
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
  m_attribute_handler.ClearFailedConstraints();
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
  return m_attribute_handler.HasStringAttribute(name);
}

std::string Procedure::GetAttributeString(const std::string &name) const
{
  return GetStringAttributeValue(m_attribute_handler.GetStringAttributes(), name);
}

bool Procedure::AddAttribute(const std::string &name, const std::string &value)
{
  return m_attribute_handler.AddStringAttribute(name, value);
}

bool Procedure::AddAttributes(const StringAttributeList& str_attributes)
{
  bool result = true;
  for (auto &attr : str_attributes)
  {
    result = AddAttribute(attr.first, attr.second) && result;
  }
  return result;
}

const Workspace* Procedure::GetWorkspace() const
{
  return m_workspace.get();
}

Workspace* Procedure::GetWorkspace()
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

bool Procedure::RegisterGenericCallback(const GenericCallback& cb)
{
  return m_workspace->RegisterGenericCallback(cb);
}

void Procedure::SetParentProcedure(Procedure* parent)
{
  m_parent = parent;
}

const ProcedureStore& Procedure::GetProcedureStore() const
{
  if (m_parent)
  {
    return m_parent->GetProcedureStore();
  }
  return m_procedure_store;
}

static bool HasRootAttributeSet(const Instruction &instruction)
{
  if (!instruction.HasAttribute(kIsRootAttribute))
  {
    return false;
  }
  auto attr_val = instruction.GetAttributeString(kIsRootAttribute);
  auto parsed = utils::ParseAttributeString(sup::dto::BooleanType, attr_val);
  if (!parsed.first)
  {
    return false;
  }
  return parsed.second.As<bool>();
}

int TickTimeoutMs(Procedure& procedure)
{
  if (procedure.HasAttribute(kTickTimeoutAttributeName))
  {
    auto tick_timeout = procedure.GetAttributeValue<double>(kTickTimeoutAttributeName);
    if (tick_timeout > 0.001)
    {
      return static_cast<int>(tick_timeout * 1000);
    }
  }
  return DefaultSettings::DEFAULT_SLEEP_TIME_MS;
}

}  // namespace sequencer

}  // namespace sup
