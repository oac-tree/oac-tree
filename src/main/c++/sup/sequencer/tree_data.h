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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_TREE_DATA_H_
#define SUP_SEQUENCER_TREE_DATA_H_

#include <sup/sequencer/attribute_map.h>

#include <string>
#include <utility>
#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief Data representation of an XML tree.
 */

class TreeData
{
private:
  std::string _type;
  std::string _content;
  AttributeMap _attributes;
  std::vector<TreeData> _children;

protected:
public:
  /**
   * @brief Constructor.
   * @param type Data typename.
   */
  TreeData(const std::string& type);

  /**
   * @brief Destructor.
   */
  ~TreeData();

  /**
   * @brief Copy/move Constructor.
   */
  TreeData(const TreeData& other);
  TreeData(TreeData&& other);

  /**
   * @brief Copy/move Assignment.
   */
  TreeData& operator=(const TreeData& other);
  TreeData& operator=(TreeData&& other);

  /**
   * @brief Comparison operators.
   */
  bool operator==(const TreeData& other) const;
  bool operator!=(const TreeData& other) const;

  /**
   * @brief Retrieve the data's typename.
   *
   * @return Typename of the data.
   */
  std::string GetType() const;

  /**
   * @brief Retrieve the data's name attribute.
   *
   * @return Name of the data.
   */
  std::string GetName() const;

  /**
   * @brief Set the data's name attribute.
   *
   * @param name Name of the data.
   * @details Overwrites if a name already existed.
   */
  void SetName(const std::string& name);

  /**
   * @brief Get number of attributes.
   *
   * @return Number of attributes.
   */
  size_t GetNumberOfAttributes() const;

  /**
   * @brief Indicate presence of attribute with given name.
   *
   * @param name Attribute name.
   * @return true when present.
   */
  bool HasAttribute(const std::string& name) const;

  /**
   * @brief Get attribute with given name.
   *
   * @param name Attribute name.
   * @return Attribute value.
   */
  std::string GetAttribute(const std::string& name) const;

  /**
   * @brief Retrieve map of all attributes.
   *
   * @return Map of all attributes.
   */
  const AttributeMap& Attributes() const;

  /**
   * @brief Add attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true on successful adding.
   * @details This method will not try to overwrite an existing attribute. It returns
   * false if the attribute with the given name already exists.
   */
  bool AddAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Set attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @details This method will overwrite an existing attribute or create a new one.
   */
  void SetAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Remove attribute with given name.
   *
   * @param name Name of attribute to remove.
   * @return true when attribute was found and removed.
   */
  bool RemoveAttribute(const std::string& name);

  /**
   * @brief Get number of children.
   *
   * @return Number of children.
   */
  size_t GetNumberOfChildren() const;

  /**
   * @brief Add child element data.
   *
   * @param child Data representation of child element.
   */
  void AddChild(const TreeData& child);

  /**
   * @brief Retrieve all child data elements.
   *
   * @return List of child data elements.
   */
  const std::vector<TreeData>& Children() const;

  /**
   * @brief Set element content string.
   *
   * @param content Content string.
   * @details Overwrites if already present.
   */
  void SetContent(const std::string& content);

  /**
   * @brief Retrieve element content string.
   *
   * @return Content string.
   */
  std::string GetContent() const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_TREE_DATA_H_
