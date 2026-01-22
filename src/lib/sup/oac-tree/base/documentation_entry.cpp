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
 * Copyright (c) : 2010-2026 ITER Organization,
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

DocumentationEntry::DocumentationEntry()
  : DocumentationEntry{std::map<std::string, std::string>{}}
{}

DocumentationEntry::DocumentationEntry(std::map<std::string, std::string> doc_map)
  : m_doc_map{std::move(doc_map)}
{}

DocumentationEntry::~DocumentationEntry() = default;

std::string DocumentationEntry::GetField(const std::string& field_name,
                                         const std::string& default_val) const
{
  auto iter = m_doc_map.find(field_name);
  if (iter == m_doc_map.end())
  {
    return default_val;
  }
  return iter->second;
}

std::string DocumentationEntry::GetField(const std::string& field_name) const
{
  return GetField(field_name, {});
}

void DocumentationEntry::SetField(const std::string& field_name, const std::string& field_val)
{
  m_doc_map[field_name] = field_val;
}

std::vector<std::string> DocumentationEntry::GetFieldNames() const
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
