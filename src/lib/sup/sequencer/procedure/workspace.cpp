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

#include <sup/sequencer/workspace.h>

#include <sup/sequencer/exceptions.h>

#include <sup/dto/anytype_registry.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <condition_variable>
#include <set>
#include <mutex>
#include <utility>

namespace sup
{
namespace sequencer
{
Workspace::Workspace(const std::string& filename)
   : m_filename{filename}
   , m_var_map{}
   , m_var_names{}
   , m_callbacks{}
   , m_type_registry{new sup::dto::AnyTypeRegistry()}
   , m_teardown_actions{}
   , m_setup_done{false}
{}

Workspace::~Workspace()
{
  for (const auto& teardown_action : m_teardown_actions)
  {
    teardown_action();
  }
}

std::string Workspace::GetFilename() const
{
  return m_filename;
}

bool Workspace::AddVariable(const std::string& name, std::unique_ptr<Variable>&& var)
{
  if (ContainsVariableName(name))
  {
    std::string error_message =
      "Workspace::AddVariable(): trying to add variable with name that already "
      "exists: [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  var->SetNotifyCallback(
    [this, name](const sup::dto::AnyValue& value, bool connected)
    {
      VariableUpdated(name, value, connected);
    });
  m_var_map[name] = std::move(var);
  m_var_names.push_back(name);
  return true;
}

std::vector<std::string> Workspace::VariableNames() const
{
  return m_var_names;
}

void Workspace::Setup()
{
  if (m_setup_done)
  {
    return;
  }
  std::vector<SetupTeardownActions> setup_teardown_actions;
  auto setup_var = [this, &setup_teardown_actions](const auto &pair) {
    auto actions = pair.second->Setup(*this);
    if (!actions.m_identifier.empty()) {
      setup_teardown_actions.push_back(actions);
    }
  };
  std::for_each(m_var_map.begin(), m_var_map.end(), setup_var);
  // call registered global setup functions
  auto setup_actions = ParseSetupTeardownActions(setup_teardown_actions);
  for (const auto& setup_action : setup_actions)
  {
    setup_action();
  }
  m_setup_done = true;
}

void Workspace::Teardown()
{
  m_setup_done = false;
  for (const auto& teardown_action : m_teardown_actions)
  {
    teardown_action();
  }
  m_teardown_actions.clear();
  std::for_each(m_var_map.begin(), m_var_map.end(), [](const auto &pair) {
     return pair.second->Teardown(); });
}

bool Workspace::ResetVariable(const std::string& varname)
{
  auto it = m_var_map.find(varname);
  if (it == m_var_map.end())
  {
    return false;
  }
  it->second->Reset(*this);
  return true;
}

bool Workspace::GetValue(const std::string& name, sup::dto::AnyValue &value) const
{
  auto [varname, fieldname] = SplitFieldName(name);

  auto it = m_var_map.find(varname);
  if (it == m_var_map.end())
  {
    return false;
  }
  return it->second->GetValue(value, fieldname);
}

bool Workspace::SetValue(const std::string& name, const sup::dto::AnyValue &value)
{
  auto [varname, fieldname] = SplitFieldName(name);

  auto it = m_var_map.find(varname);
  if (it == m_var_map.end())
  {
    return false;
  }
  return it->second->SetValue(value, fieldname);
}

bool Workspace::WaitForVariable(const std::string& name, double timeout_sec, bool availability)
{
  auto it = m_var_map.find(name);
  if (it == m_var_map.end())
  {
    return false;
  }
  auto timeout_duration = std::chrono::duration<double>(timeout_sec);
  int dummy_listener; // to get a unique address
  std::mutex mx;
  std::unique_lock<std::mutex> lk(mx);
  std::condition_variable cv;
  auto cb_guard = GetCallbackGuard(&dummy_listener);
  auto callback = [&cv](const auto&, auto){
                    cv.notify_one();
                  };
  RegisterCallback(name, callback, &dummy_listener);
  cv.wait_for(lk, timeout_duration, [&it, availability]{
      return it->second->IsAvailable() == availability;
    });
  return it->second->IsAvailable() == availability;
}

std::vector<const Variable*> Workspace::GetVariables() const
{
  std::vector<const Variable*> result;
  for (const auto& var_name : m_var_names)
  {
    result.push_back(GetVariable(var_name));
  }
  return result;
}

const Variable* Workspace::GetVariable(const std::string& name) const
{
  auto it = m_var_map.find(name);
  if (it == m_var_map.end())
  {
    return nullptr;
  }
  return it->second.get();
}

bool Workspace::HasVariable(const std::string& name) const
{
  return m_var_map.find(name) != m_var_map.end();
}

bool Workspace::RegisterType(const sup::dto::AnyType& anytype)
{
  if (m_type_registry->HasType(anytype.GetTypeName()))
  {
    return anytype == m_type_registry->GetType(anytype.GetTypeName());
  }
  m_type_registry->RegisterType(anytype);
  return true;
}

const sup::dto::AnyTypeRegistry& Workspace::GetTypeRegistry() const
{
  return *m_type_registry;
}

ScopeGuard Workspace::GetCallbackGuard(void *listener)
{
  return m_callbacks.GetCallbackGuard(listener);
}

bool Workspace::RegisterGenericCallback(const GenericCallback& cb, void* listener)
{
  return m_callbacks.RegisterGenericCallback(cb, listener);
}

bool Workspace::RegisterCallback(const std::string& name, const VariableCallback& cb,
                                 void *listener)
{
  return m_callbacks.RegisterCallback(name, cb, listener);
}

bool Workspace::IsSuccessfullySetup() const
{
  return m_setup_done;
}

bool Workspace::ContainsVariableName(const std::string& name) const
{
  if (m_var_map.find(name) == m_var_map.end())
  {
    return false;
  }
  return true;
}

std::vector<std::function<void()>> Workspace::ParseSetupTeardownActions(
    const std::vector<SetupTeardownActions>& actions)
{
  m_teardown_actions.clear();
  std::set<std::string> setup_identifiers;
  std::set<std::string> teardown_identifiers;
  std::vector<std::function<void()>> setup_actions;
  for (const auto& action : actions)
  {
    if (action.m_identifier.empty())
    {
      continue;
    }

    auto setup_iter =
      std::find(setup_identifiers.begin(), setup_identifiers.end(), action.m_identifier);
    auto teardown_iter =
      std::find(teardown_identifiers.begin(), teardown_identifiers.end(), action.m_identifier);
    if (action.m_setup && setup_iter == setup_identifiers.end())
    {
      setup_actions.push_back(action.m_setup);
      setup_identifiers.insert(action.m_identifier);
    }
    if (action.m_teardown && teardown_iter == teardown_identifiers.end())
    {
      m_teardown_actions.push_back(action.m_teardown);
      teardown_identifiers.insert(action.m_identifier);
    }
  }
  return setup_actions;
}

void Workspace::VariableUpdated(const std::string name, const sup::dto::AnyValue& value,
                                bool connected) const
{
  m_callbacks.ExecuteCallbacks(name, value, connected);
}

std::pair<std::string, std::string> SplitFieldName(const std::string& fullname)
{
  auto pos1 = fullname.find('.');
  auto pos2 = fullname.find('[');
  auto end_first = std::min(pos1, pos2);
  if (end_first == std::string::npos)
  {
    return {fullname, {}};
  }
  // Do not remove the '[' character from the next string part:
  auto start_next = pos1 < pos2 ? pos1 + 1 : pos2;
  return {fullname.substr(0u, end_first), fullname.substr(start_next)};
}

}  // namespace sequencer

}  // namespace sup
