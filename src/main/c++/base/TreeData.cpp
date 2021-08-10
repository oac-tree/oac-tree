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

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

TreeData::TreeData(const std::string& type) : _type{type} {}

TreeData::~TreeData() = default;

TreeData::TreeData(const TreeData& other) = default;
TreeData::TreeData(TreeData&& other) = default;

TreeData& TreeData::operator=(const TreeData& other) = default;
TreeData& TreeData::operator=(TreeData&& other) = default;

bool TreeData::operator==(const TreeData& other) const
{
  bool result = (_type == other._type) && (_content == other._content)
                && (_attributes == other._attributes) && (_children == other._children);
  return result;
}

bool TreeData::operator!=(const TreeData& other) const
{
  return !this->operator==(other);
}

std::string TreeData::GetType() const
{
  return _type;
}

std::string TreeData::GetName() const
{
  return GetAttribute(attributes::NAME_ATTRIBUTE);
}

void TreeData::SetName(const std::string& name)
{
  return SetAttribute(attributes::NAME_ATTRIBUTE, name);
}

size_t TreeData::GetNumberOfAttributes() const
{
  return _attributes.GetNumberOfAttributes();
}

bool TreeData::HasAttribute(const std::string& name) const
{
  return _attributes.HasAttribute(name);
}

std::string TreeData::GetAttribute(const std::string& name) const
{
  return _attributes.GetAttribute(name);
}

const AttributeMap& TreeData::Attributes() const
{
  return _attributes;
}

bool TreeData::AddAttribute(const std::string& name, const std::string& value)
{
  return _attributes.AddAttribute(name, value);
}

void TreeData::SetAttribute(const std::string& name, const std::string& value)
{
  return _attributes.SetAttribute(name, value);
}

bool TreeData::RemoveAttribute(const std::string& name)
{
  return _attributes.Remove(name);
}

size_t TreeData::GetNumberOfChildren() const
{
  return _children.size();
}

void TreeData::AddChild(const TreeData& child)
{
  _children.push_back(child);
}

const std::vector<TreeData>& TreeData::Children() const
{
  return _children;
}

void TreeData::SetContent(const std::string& content)
{
  _content = content;
}

std::string TreeData::GetContent() const
{
  return _content;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
