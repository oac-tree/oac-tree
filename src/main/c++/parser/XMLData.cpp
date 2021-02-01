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

#include "XMLData.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

XMLData::XMLData(std::string type)
  : _type{std::move(type)}
{}

XMLData::~XMLData() = default;

std::string XMLData::GetType() const
{
  return _type;
}

std::string XMLData::GetName() const
{
  if (HasAttribute(NAME_ATTRIBUTE))
  {
    return _attributes.at(NAME_ATTRIBUTE);
  }
  return {};
}

bool XMLData::HasAttribute(const std::string & name) const
{
  return _attributes.find(name) != _attributes.end();
}

bool XMLData::AddAttribute(const std::string & name, const std::string & value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  _attributes[name] = value;
}

const std::map<std::string, std::string> & XMLData::Attributes() const
{
  return _attributes;
}

bool XMLData::AddChild(const XMLData & child)
{
  _children.push_back(child);
}

const std::vector<XMLData> & XMLData::Children() const
{
  return _children;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
