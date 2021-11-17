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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files
#include <algorithm>
#include <memory>
#include <utility>

#include <common/log-api.h>

// Local header files

#include "Workspace.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

static std::pair<std::string, std::string> SplitToNameField(const std::string &fullname);

// Function definition

Workspace::Workspace() : _var_map{}, _var_pointers{} {}

Workspace::~Workspace()
{
  for (auto var : _var_pointers)
  {
    delete var;
  }
}

bool Workspace::AddVariable(std::string name, Variable *var)
{
  std::unique_ptr<Variable> var_owned(var);  // take ownership immediately
  if (_var_map.find(name) != _var_map.end())
  {
    log_warning(
        "sup::sequencer::Workspace::AddVariable('%s', var) - variable with "
        "this name already exists!",
        name.c_str());
    return false;
  }
  auto result = _var_pointers.insert(var);
  if (result.second)
  {
    log_debug(
        "sup::sequencer::Workspace::AddVariable('%s', var) - add variable "
        "to workspace..",
        name.c_str());
    _var_map[name] = var_owned.release();
  }
  else
  {
    var_owned.release();  // variable is already owned
    log_warning(
        "sup::sequencer::Workspace::AddVariable('%s', var) - variable with "
        "same address already added!",
        name.c_str());
  }
  return result.second;
}

std::vector<std::string> Workspace::VariableNames() const
{
  std::vector<std::string> result;
  std::transform(_var_map.begin(), _var_map.end(), std::back_inserter(result),
                 [](const decltype(_var_map)::value_type &pair) { return pair.first; });
  return result;
}

bool Workspace::GetValue(std::string name, ::ccs::types::AnyValue &value) const
{
  auto splitname = SplitToNameField(name);
  auto varname = splitname.first;
  auto fieldname = splitname.second;

  auto it = _var_map.find(varname);
  if (it == _var_map.end())
  {
    log_warning(
        "sup::sequencer::Workspace::GetValue('%s', value) - variable with name '%s' "
        "not in workspace!",
        name.c_str(), varname.c_str());
    return false;
  }
  auto var = it->second;

  log_debug(
      "sup::sequencer::Workspace::GetValue('%s', 'value') - trying to copy found "
      "workspace variable to 'value'..",
      name.c_str());
  return var->GetValue(value, fieldname);
}

bool Workspace::SetValue(std::string name, const ::ccs::types::AnyValue &value)
{
  auto splitname = SplitToNameField(name);
  auto varname = splitname.first;
  auto fieldname = splitname.second;

  auto it = _var_map.find(varname);
  if (it == _var_map.end())
  {
    log_warning(
        "sup::sequencer::Workspace::SetValue('%s', value) - variable with name '%s' "
        "not in workspace!",
        name.c_str(), varname.c_str());
    return false;
  }
  auto var = it->second;
  log_debug(
      "sup::sequencer::Workspace::SetValue('%s', 'value') - trying to copy "
      "'value' into found workspace variable..",
      name.c_str());

  return var->SetValue(value, fieldname);
}

std::vector<const Variable *> Workspace::GetVariables() const
{
  std::vector<const Variable *> result;
  std::transform(std::begin(_var_map), std::end(_var_map), std::back_inserter(result),
                 [](const decltype(_var_map)::value_type &pair) { return pair.second; });
  return result;
}

const Variable* Workspace::GetVariable(const std::string& name) const
{
  auto it = _var_map.find(name);
  if (it == _var_map.end())
  {
    return nullptr;
  }
  return it->second;
}

bool Workspace::HasVariable(const std::string& name) const
{
  return _var_map.find(name) != _var_map.end();
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

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
