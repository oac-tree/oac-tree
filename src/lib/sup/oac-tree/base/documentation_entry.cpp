/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/documentation_entry.h>

namespace sup
{
namespace oac_tree
{

DocomentationEntry::DocomentationEntry()
  : DocomentationEntry{std::map<std::string, std::string>{}}
{}

DocomentationEntry::DocomentationEntry(std::map<std::string, std::string> doc_map)
  : m_doc_map{std::move(doc_map)}
{}

DocomentationEntry::~DocomentationEntry() = default;

std::string DocomentationEntry::GetField(const std::string& field_name,
                                     const std::string& default_val) const
{
  auto iter = m_doc_map.find(field_name);
  if (iter == m_doc_map.end())
  {
    return default_val;
  }
  return iter->second;
}

std::string DocomentationEntry::GetField(const std::string& field_name) const
{
  return GetField(field_name, {});
}

std::vector<std::string> DocomentationEntry::GetFieldNames() const
{
  std::vector<std::string> result{};
  for (const auto& [name, _] : m_doc_map)
  {
    result.push_back(name);
  }
  return result;
}

}  // namespace oac_tree

}  // namespace sup
