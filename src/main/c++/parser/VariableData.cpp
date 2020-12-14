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

// Local header files

#include "VariableData.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

VariableData::VariableData(std::string var_type)
  : _type{std::move(var_type)}
{}

std::string VariableData::GetType() const
{
  return _type;
}

std::string VariableData::GetName() const
{
  if (HasAttribute(VARIABLE_NAME_ATTRIBUTE))
  {
    return _attributes.at(VARIABLE_NAME_ATTRIBUTE);
  }
  return {};
}

bool VariableData::HasAttribute(const std::string &name) const
{
  return _attributes.find(name) != _attributes.end();
}

bool VariableData::AddAttribute(const std::string &name, const std::string &value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  _attributes[name] = value;
}

const std::map<std::string, std::string> & VariableData::Attributes() const
{
  return _attributes;
}

std::unique_ptr<Variable> VariableData::GenerateVariable() const
{
  // auto instr = GlobalInstructionRegistry().Create(_type);
  // if (!instr)
  // {
  //   return {};
  // }
  // for (const auto & attr : _attributes)
  // {
  //   instr->AddAttribute(attr.first, attr.second);
  // }
  return {};
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
