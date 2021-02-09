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

// Local header files

#include "AttributeMap.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

AttributeMap::AttributeMap() = default;

AttributeMap::~AttributeMap() = default;

bool AttributeMap::HasAttribute(const std::string & name) const
{
  return _attributes.find(name) != _attributes.end();
}

std::string AttributeMap::GetAttribute(const std::string & name) const
{
  if (!HasAttribute(name))
  {
      return {};
  }
  return _attributes.at(name);
}

std::vector<std::string> AttributeMap::GetAttributeNames() const
{
  std::vector<std::string> result;
  for (auto & pair : _attributes)
  {
    result.push_back(pair.first);
  }
  return result;
}

std::vector<std::pair<const std::string, std::string>> AttributeMap::AttributeList() const
{
  std::vector<std::pair<const std::string, std::string>> result;
  for (auto & pair : _attributes)
  {
    result.push_back(pair);
  }
  return result;
}

bool AttributeMap::AddAttribute(const std::string & name, const std::string & value)
{
  if (HasAttribute(name))
  {
    return false;
  }
  _attributes[name] = value;
  return true;
}

void AttributeMap::Clear()
{
  _attributes.clear();
}

bool AttributeMap::Remove(const std::string & name)
{
  return _attributes.erase(name) > 0;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
