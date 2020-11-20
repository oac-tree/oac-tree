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

#include <common/log-api.h>
#include <utility>

// Local header files

#include "Workspace.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

Workspace::Workspace()
  : _var_map{}
{}

Workspace::~Workspace() = default;

bool Workspace::AddVariable(Variable var)
{
  if (_var_map.find(var.GetName()) != _var_map.end())
  {
    return false;
  }
  _var_map.insert(std::map<std::string, Variable>::value_type(var.GetName(), var));
  return true;
}

std::vector<std::string> Workspace::VariableNames() const
{
  std::vector<std::string> result;
  for(const auto & entry : _var_map)
  {
    auto name = entry.first;
    result.push_back(name);
  }
  return result;
}

int Workspace::GetVariableValue(std::string name)
{
  auto it = _var_map.find(name);
  if (it == _var_map.end())
  {
    return 0;
  }
  auto var = it->second;
  return var.GetValue();
}

bool Workspace::SetVariableValue(std::string name, int value)
{
  auto it = _var_map.find(name);
  if (it == _var_map.end())
  {
    return false;
  }
  auto& var = it->second;
  var.SetValue(value);
  return true;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
