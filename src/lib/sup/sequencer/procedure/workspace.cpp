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

#include <sup/sequencer/workspace.h>

#include <sup/sequencer/exceptions.h>

#include <sup/dto/anytype_registry.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <utility>

namespace sup
{
namespace sequencer
{
bool Workspace::ContainsVariableName(const std::string& name) const
{
  if (m_var_map.find(name) == m_var_map.end())
  {
    return false;
  }
  return true;
}

bool Workspace::ContainsVariablePointer(Variable* var) const
{
  auto it = std::find_if(m_var_map.begin(), m_var_map.end(),
                         [var](const decltype(m_var_map)::value_type &pair)
                         {
                           return pair.second.get() == var;
                         });
  if (it == m_var_map.end())
  {
    return false;
  }
  return true;
}

void Workspace::VariableUpdated(const std::string name, const sup::dto::AnyValue& value,
                                bool connected) const
{
  m_callbacks.ExecuteCallbacks(name, value, connected);
}

Workspace::Workspace()
   : m_var_map{}
   , m_callbacks{}
   , m_type_registry{new sup::dto::AnyTypeRegistry()}
{}

Workspace::~Workspace() = default;

bool Workspace::AddVariable(const std::string& name, Variable *var)
{
  std::unique_ptr<Variable> var_owned(var);  // take ownership immediately
  if (ContainsVariablePointer(var))
  {
    auto vartype = var_owned->GetType();
    var_owned.release(); // do not delete this variable!
    std::string error_message =
      "sup::sequencer::Workspace::AddVariable(): trying to add variable that already exists, "
      "with type [" + vartype + "] and name [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  if (ContainsVariableName(name))
  {
    std::string error_message =
      "sup::sequencer::Workspace::AddVariable(): trying to add variable with name that already "
      "exists: [" + name + "]";
    throw InvalidOperationException(error_message);
  }
  var_owned->SetNotifyCallback(
    [this, name](const sup::dto::AnyValue& value, bool connected)
    {
      VariableUpdated(name, value, connected);
    });
  m_var_map[name] = std::move(var_owned);
  return true;
}

std::vector<std::string> Workspace::VariableNames() const
{
  std::vector<std::string> result;
  std::transform(m_var_map.begin(), m_var_map.end(), std::back_inserter(result),
                 [](const decltype(m_var_map)::value_type &pair) { return pair.first; });
  return result;
}

void Workspace::Setup()
{
  const sup::dto::AnyTypeRegistry* registry = m_type_registry.get();
  std::for_each(m_var_map.begin(), m_var_map.end(),
                [registry](const decltype(m_var_map)::value_type &pair) {
                  return pair.second->Setup(registry);
                });
}

void Workspace::Reset()
{
  std::for_each(m_var_map.begin(), m_var_map.end(), [](const decltype(m_var_map)::value_type &pair) {
     return pair.second->Reset(); });
}

bool Workspace::ResetVariable(const std::string& varname)
{
  auto it = m_var_map.find(varname);
  if (it == m_var_map.end())
  {
    return false;
  }
  it->second->Reset();
  it->second->Setup(m_type_registry.get());
  return true;
}

bool Workspace::GetValue(const std::string& name, sup::dto::AnyValue &value) const
{
  auto splitname = SplitFieldName(name);
  auto varname = splitname.first;
  auto fieldname = splitname.second;

  auto it = m_var_map.find(varname);
  if (it == m_var_map.end())
  {
    return false;
  }
  return it->second->GetValue(value, fieldname);
}

bool Workspace::SetValue(const std::string& name, const sup::dto::AnyValue &value)
{
  auto splitname = SplitFieldName(name);
  auto varname = splitname.first;
  auto fieldname = splitname.second;

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
  long timeout_ns = std::lround(timeout_sec * 1e9);
  auto time_end = std::chrono::system_clock::now() + std::chrono::nanoseconds(timeout_ns);
  int dummy_listener; // to get a unique address
  std::mutex mx;
  std::unique_lock<std::mutex> lk(mx);
  std::condition_variable cv;
  auto cb_guard = GetCallbackGuard(&dummy_listener);
  auto callback = [&cv](const sup::dto::AnyValue&, bool){
                    cv.notify_one();
                  };
  RegisterCallback(name, callback, &dummy_listener);
  cv.wait_until(lk, time_end, [&it, availability]{
      return it->second->IsAvailable() == availability;
    });
  return it->second->IsAvailable() == availability;
}

std::vector<const Variable*> Workspace::GetVariables() const
{
  std::vector<const Variable*> result;
  std::transform(std::begin(m_var_map), std::end(m_var_map), std::back_inserter(result),
                 [](const decltype(m_var_map)::value_type &pair) { return pair.second.get(); });
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

const sup::dto::AnyTypeRegistry* Workspace::GetTypeRegistry() const
{
  return m_type_registry.get();
}

ScopeGuard Workspace::GetCallbackGuard(
    void *listener)
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

std::pair<std::string, std::string> SplitFieldName(const std::string& fullname)
{
  auto pos1 = fullname.find('.');
  auto pos2 = fullname.find('[');
  auto pos = std::min(pos1, pos2);

  auto total_length = fullname.length();
  if (pos < total_length)
  {
    return {fullname.substr(0u, pos), fullname.substr(pos + 1u)};
  }
  return {fullname, {}};
}

}  // namespace sequencer

}  // namespace sup
