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

#include <sup/sequencer/log.h>

#include <algorithm>
#include <utility>

namespace sup
{
namespace sequencer
{
static std::pair<std::string, std::string> SplitToNameField(const std::string &fullname);

bool Workspace::ContainsVariableName(const std::string& name) const
{
  if (_var_map.find(name) == _var_map.end())
  {
    return false;
  }
  return true;
}

bool Workspace::ContainsVariablePointer(Variable* var) const
{
  auto it = std::find_if(_var_map.begin(), _var_map.end(),
                         [var](const decltype(_var_map)::value_type &pair)
                         {
                           return pair.second.get() == var;
                         });
  if (it == _var_map.end())
  {
    return false;
  }
  return true;
}

void Workspace::VariableUpdated(const std::string name, const ccs::types::AnyValue& value)
{
  callbacks.ExecuteCallbacks(name, value);
}

Workspace::Workspace()
   : _var_map{}
{}

Workspace::~Workspace() = default;

bool Workspace::AddVariable(std::string name, Variable *var)
{
  std::unique_ptr<Variable> var_owned(var);  // take ownership immediately
  if (ContainsVariablePointer(var))
  {
    log::Warning(
        "sup::sequencer::Workspace::AddVariable('%s', var) - variable pointer already exists!",
        name.c_str());
    var_owned.release(); // do not delete this variable!
    return false;
  }
  if (ContainsVariableName(name))
  {
    log::Warning(
        "sup::sequencer::Workspace::AddVariable('%s', var) - variable name already exists!",
        name.c_str());
    return false;
  }
  log::Debug("sup::sequencer::Workspace::AddVariable('%s', var) - add variable to workspace..",
            name.c_str());
  var_owned->SetNotifyCallback(
    [this, name](const ccs::types::AnyValue& value)
    {
      VariableUpdated(name, value);
    });
  _var_map[name] = std::move(var_owned);
  return true;
}

std::vector<std::string> Workspace::VariableNames() const
{
  std::vector<std::string> result;
  std::transform(_var_map.begin(), _var_map.end(), std::back_inserter(result),
                 [](const decltype(_var_map)::value_type &pair) { return pair.first; });
  return result;
}

void Workspace::Setup()
{
  std::for_each(_var_map.begin(), _var_map.end(), [](const decltype(_var_map)::value_type &pair) {
     return pair.second->Setup(); });
}

void Workspace::Reset()
{
  std::for_each(_var_map.begin(), _var_map.end(), [](const decltype(_var_map)::value_type &pair) {
     return pair.second->Reset(); });
}

bool Workspace::ResetVariable(const std::string& varname)
{
  auto it = _var_map.find(varname);
  if (it == _var_map.end())
  {
    return false;
  }
  it->second->Reset();
  it->second->Setup();
  return true;
}

bool Workspace::GetValue(std::string name, ::ccs::types::AnyValue &value) const
{
  auto splitname = SplitToNameField(name);
  auto varname = splitname.first;
  auto fieldname = splitname.second;

  auto it = _var_map.find(varname);
  if (it == _var_map.end())
  {
    log::Warning(
        "sup::sequencer::Workspace::GetValue('%s', value) - variable with name '%s' "
        "not in workspace!",
        name.c_str(), varname.c_str());
    return false;
  }
  log::Debug(
      "sup::sequencer::Workspace::GetValue('%s', 'value') - trying to copy found "
      "workspace variable to 'value'..",
      name.c_str());
  return it->second->GetValue(value, fieldname);
}

bool Workspace::SetValue(std::string name, const ::ccs::types::AnyValue &value)
{
  auto splitname = SplitToNameField(name);
  auto varname = splitname.first;
  auto fieldname = splitname.second;

  auto it = _var_map.find(varname);
  if (it == _var_map.end())
  {
    log::Warning(
        "sup::sequencer::Workspace::SetValue('%s', value) - variable with name '%s' "
        "not in workspace!",
        name.c_str(), varname.c_str());
    return false;
  }
  log::Debug(
      "sup::sequencer::Workspace::SetValue('%s', 'value') - trying to copy "
      "'value' into found workspace variable..",
      name.c_str());

  return  it->second->SetValue(value, fieldname);
}

std::vector<const Variable *> Workspace::GetVariables() const
{
  std::vector<const Variable *> result;
  std::transform(std::begin(_var_map), std::end(_var_map), std::back_inserter(result),
                 [](const decltype(_var_map)::value_type &pair) { return pair.second.get(); });
  return result;
}

const Variable* Workspace::GetVariable(const std::string& name) const
{
  auto it = _var_map.find(name);
  if (it == _var_map.end())
  {
    return nullptr;
  }
  return it->second.get();
}

bool Workspace::HasVariable(const std::string& name) const
{
  return _var_map.find(name) != _var_map.end();
}

CallbackGuard<NamedCallbackManager<const ccs::types::AnyValue &>> Workspace::GetCallbackGuard(
    void *listener)
{
  return callbacks.GetCallbackGuard(this);
}

bool Workspace::RegisterGenericCallback(
    const std::function<void(const std::string &, const ccs::types::AnyValue &)> &cb,
    void *listener)
{
  return callbacks.RegisterGenericCallback(cb, listener);
}

bool Workspace::RegisterCallback(const std::string &name,
                                 const std::function<void(const ccs::types::AnyValue &)> &cb,
                                 void *listener)
{
  return callbacks.RegisterCallback(name, cb, listener);
}

static std::pair<std::string, std::string> SplitToNameField(const std::string &fullname)
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
