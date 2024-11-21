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
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sup/sequencer/instructions/instruction_helper.h>
#include <sup/sequencer/parser/procedure_parser.h>
#include <sup/sequencer/procedure/procedure_store.h>

#include <sup/sequencer/attribute_utils.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/instruction_tree.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>
#include <sup/dto/anytype_helper.h>
#include <sup/dto/json_type_parser.h>

namespace
{
using sup::sequencer::Instruction;
bool HasRootAttributeSet(const Instruction &instruction);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{

Procedure::Procedure(const std::string& filename)
  : m_instructions{}
  , m_workspace{new Workspace(filename)}
  , m_attribute_handler{}
  , m_preamble{}
  , m_parent{}
  , m_procedure_store{new ProcedureStore{this}}
{
  m_attribute_handler.AddAttributeDefinition(kTickTimeoutAttributeName, sup::dto::Float64Type);
}

Procedure::~Procedure()
{
  // Wait for termination of instructions first:
  DefaultUserInterface ui;
  Teardown(ui);
}

std::string Procedure::GetFilename() const
{
  return m_workspace->GetFilename();
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

ProcedureContext Procedure::GetContext() const
{
  return ProcedureContext(GetFilename(), GetProcedureStore());
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

void Procedure::Setup()
{
  if (!m_attribute_handler.ValidateAttributes())
  {
    std::string error_message = "Procedure::Setup(): Failed attribute constraint(s):" +
                                FormatFailedConstraints(m_attribute_handler.GetFailedConstraints());
    throw ProcedureSetupException(error_message);
  }
  SetupPreamble();
  m_workspace->Setup();
  if (RootInstruction() == nullptr)
  {
    std::string error_message = "Procedure::Setup(): No root instruction";
    throw ProcedureSetupException(error_message);
  }
  RootInstruction()->Setup(*this);
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

void Procedure::Reset(UserInterface& ui)
{
  m_workspace->Teardown();
  m_procedure_store->ResetProcedureWorkspaces(ui);
  if (RootInstruction() != nullptr)
  {
    RootInstruction()->Reset(ui);
  }
  m_workspace->Setup();
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

const Workspace& Procedure::GetWorkspace() const
{
  return *m_workspace;
}

Workspace& Procedure::GetWorkspace()
{
  return *m_workspace;
}

const ProcedurePreamble& Procedure::GetPreamble() const
{
  return m_preamble;
}

ProcedurePreamble& Procedure::GetPreamble()
{
  return m_preamble;
}

bool Procedure::RegisterType(const sup::dto::AnyType& anytype)
{
  return m_workspace->RegisterType(anytype);
}

const sup::dto::AnyTypeRegistry& Procedure::GetTypeRegistry() const
{
  return m_workspace->GetTypeRegistry();
}

bool Procedure::RegisterGenericCallback(const GenericCallback& cb, void* listener)
{
  return m_workspace->RegisterGenericCallback(cb, listener);
}

void Procedure::SetParentProcedure(Procedure* parent)
{
  m_parent = parent;
}

void Procedure::SetupPreamble()
{
  for (const auto& type_registration : m_preamble.GetTypeRegistrations())
  {
    auto anytype = ParseTypeRegistrationInfo(type_registration, GetFilename(), GetTypeRegistry());
    if (!RegisterType(anytype))
    {
      std::string error_message =
        "Procedure::SetupPreamble(): type could not be registered to the registry: ["
        + sup::dto::AnyTypeToJSONString(anytype) + "]";
      throw ProcedureSetupException(error_message);
    }
  }
}

void Procedure::Teardown(UserInterface& ui)
{
  m_workspace->Teardown();
  m_procedure_store->TearDownProcedures(ui);
  if (RootInstruction() == nullptr)
  {
    return;
  }
  RootInstruction()->Reset(ui);  // instructions are reset instead of torn down.
  m_attribute_handler.ClearFailedConstraints();
}

const ProcedureStore& Procedure::GetProcedureStore() const
{
  if (m_parent)
  {
    return m_parent->GetProcedureStore();
  }
  return *m_procedure_store;
}

int TickTimeoutMs(const Procedure& procedure)
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

std::string GetProcedureName(const Procedure& procedure)
{
  if (procedure.HasAttribute(Constants::NAME_ATTRIBUTE_NAME))
  {
    return procedure.GetAttributeString(Constants::NAME_ATTRIBUTE_NAME);
  }
  return procedure.GetFilename();
}

std::vector<const Instruction*> GetNextInstructions(const Procedure& proc)
{
  auto tree = proc.GetNextInstructionTree();
  if (tree.IsEmpty())
  {
    return {};
  }
  return FlattenBFS(tree);
}

std::vector<const Instruction*> GetNextLeaves(const Procedure& proc)
{
  auto tree = proc.GetNextInstructionTree();
  if (tree.IsEmpty())
  {
    return {};
  }
  return GetLeaves(tree);
}

sup::dto::AnyType ParseTypeRegistrationInfo(const TypeRegistrationInfo& info,
                                            const std::string& filename,
                                            const sup::dto::AnyTypeRegistry& type_registry)
{
  sup::dto::JSONAnyTypeParser parser;
  if (info.GetRegistrationMode() == TypeRegistrationInfo::kJSONString)
  {
    if (!parser.ParseString(info.GetString(), std::addressof(type_registry)))
    {
      std::string error_message =
          "Procedure::SetupPreamble(): could not parse type: [" + info.GetString() + "]";
      throw ProcedureSetupException(error_message);
    }
    return parser.MoveAnyType();
  }
  if (info.GetRegistrationMode() == TypeRegistrationInfo::kJSONFile)
  {
    auto json_filename = GetFullPathName(GetFileDirectory(filename), info.GetString());
    if (!parser.ParseFile(json_filename, std::addressof(type_registry)))
    {
      std::string error_message =
          "Procedure::SetupPreamble(): could not parse file: [" + json_filename + "]";
      throw ProcedureSetupException(error_message);
    }
    return parser.MoveAnyType();
  }
  std::string error_message =
      "Procedure::SetupPreamble(): unknown type registration mode; must be from JSON file or JSON "
      "string";
  throw ProcedureSetupException(error_message);
}

std::string GetFullPathName(const std::string& directory, const std::string& filename)
{
  if (filename.empty() || filename.front() == '/' || directory.empty())
  {
    return filename;
  }
  if (directory.back() == '/')
  {
    return directory + filename;
  }
  return directory + "/" + filename;
}

std::string GetFileDirectory(const std::string& filename)
{
  auto pos = filename.find_last_of('/');
  if (pos == std::string::npos)
  {
    return {};
  }
  return filename.substr(0, pos + 1);
}

std::string GetProcedurePath(const Procedure& proc)
{
  return GetFileDirectory(proc.GetFilename());
}

std::string ResolveRelativePath(const Procedure& proc, const std::string& filename)
{
  return GetFullPathName(GetProcedurePath(proc), filename);
}

ScopeGuard GetCallbackGuard(Procedure& proc, void *listener)
{
  return proc.GetWorkspace().GetCallbackGuard(listener);
}

}  // namespace sequencer

}  // namespace sup

namespace
{
using namespace sup::sequencer;
bool HasRootAttributeSet(const Instruction &instruction)
{
  if (!instruction.HasAttribute(Constants::IS_ROOT_ATTRIBUTE_NAME))
  {
    return false;
  }
  auto attr_val = instruction.GetAttributeString(Constants::IS_ROOT_ATTRIBUTE_NAME);
  auto parsed = utils::ParseAttributeString(sup::dto::BooleanType, attr_val);
  if (!parsed.first)
  {
    return false;
  }
  return parsed.second.As<bool>();
}

}  // unnamed namespace
