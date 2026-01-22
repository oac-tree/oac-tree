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

#ifndef SUP_OAC_TREE_DOCUMENTATION_ENTRY_H_
#define SUP_OAC_TREE_DOCUMENTATION_ENTRY_H_

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief The DocumentationEntry class represents basic information about an instruction. Instances of
 * this class are expected to be registered together with instruction factory functions, to allow
 * user interfaces to display basic documentation about specific instructions.
 */
class DocumentationEntry
{
public:
  DocumentationEntry();
  explicit DocumentationEntry(std::map<std::string, std::string> doc_map);
  ~DocumentationEntry();

  // Copy/move construction/assignment
  DocumentationEntry(const DocumentationEntry& other) = default;
  DocumentationEntry(DocumentationEntry&& other) noexcept = default;
  DocumentationEntry& operator=(const DocumentationEntry& other) = default;
  DocumentationEntry& operator=(DocumentationEntry&& other) noexcept = default;

  /**
   * @brief Get the string entry referred to by a specific key.
   *
   * @param field_name Key to look for.
   * @param default_val Default value to return when key is not present.
   * @return Value registered under the given key or default_val.
   */
  std::string GetField(const std::string& field_name, const std::string& default_val) const;

  /**
   * @brief Get the string entry referred to by a specific key.
   *
   * @param field_name Key to look for.
   * @return Value registered under the given key or empty string.
   */
  std::string GetField(const std::string& field_name) const;

  /**
   * @brief Set a specific field of the documentation entry.
   *
   * @param field_name Name of the field to set.
   * @param field_val Value of the field to set.
   */
  void SetField(const std::string& field_name, const std::string& field_val);

  /**
   * @brief Retrieve a list of all the field names.
   *
   * @return List of all fieldnames.
   */
  std::vector<std::string> GetFieldNames() const;

private:
  std::map<std::string, std::string> m_doc_map;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_DOCUMENTATION_ENTRY_H_
