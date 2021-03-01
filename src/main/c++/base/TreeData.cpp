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

#include "TreeData.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

// Function definition

TreeData::TreeData(const std::string & type)
  : _type{type}
{}

TreeData::~TreeData() = default;

std::string TreeData::GetType() const
{
  return _type;
}

std::string TreeData::GetName() const
{
  return GetAttribute(NAME_ATTRIBUTE);
}

bool TreeData::HasAttribute(const std::string & name) const
{
  return _attributes.HasAttribute(name);
}

std::string TreeData::GetAttribute(const std::string & name) const
{
  return _attributes.GetAttribute(name);
}

bool TreeData::AddAttribute(const std::string & name, const std::string & value)
{
  return _attributes.AddAttribute(name, value);
}

std::vector<std::pair<const std::string, std::string>> TreeData::Attributes() const
{
  return _attributes.AttributeList();
}

bool TreeData::AddChild(const TreeData & child)
{
  _children.push_back(child);
}

const std::vector<TreeData> & TreeData::Children() const
{
  return _children;
}

bool TreeData::SetContent(std::string content)
{
  _content = std::move(content);
}

std::string TreeData::GetContent() const
{
  return _content;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
