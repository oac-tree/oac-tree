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

#include "InstructionData.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

InstructionData::InstructionData(std::string instr_type)
  : _type{std::move(instr_type)}
{}

bool InstructionData::HasAttribute(const std::string & name)
{
  return _attributes.find(name) != _attributes.end();
}

bool InstructionData::AddAttribute(const std::string & name,
                                   const std::string & value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  _attributes[name] = value;
}

bool InstructionData::AddChild(const InstructionData & child)
{
  _children.push_back(child);
}

const std::map<std::string, std::string> & InstructionData::Attributes() const
{
  return _attributes;
}

const std::vector<InstructionData> & InstructionData::Children() const
{
  return _children;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
