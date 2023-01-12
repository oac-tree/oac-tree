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

#ifndef SUP_SEQUENCER_ATTRIBUTE_MAP_H_
#define SUP_SEQUENCER_ATTRIBUTE_MAP_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace sup
{
namespace sequencer
{
using Attribute = std::pair<std::string, std::string>;

namespace attributes
{
const std::string NAME_ATTRIBUTE = "name";
bool AttributeAsBool(const std::string& value);
}

/**
 * @brief Class that contains attributes and exposes a map interface.
 */
class AttributeMap
{
public:
  using map_type = std::vector<Attribute>;

  AttributeMap();
  ~AttributeMap();

  AttributeMap(const map_type& attr_list);

  AttributeMap(const AttributeMap& other);
  AttributeMap(AttributeMap&& other);
  AttributeMap& operator=(const AttributeMap& other);
  AttributeMap& operator=(AttributeMap&& other);

  /**
   * @brief Comparison operators.
   */
  bool operator==(const AttributeMap& other) const;
  bool operator!=(const AttributeMap& other) const;

  /**
   * @brief const iterator forwarding.
   */
  using const_iterator = map_type::const_iterator;
  const_iterator begin() const { return m_attributes.begin(); }
  const_iterator end() const { return m_attributes.end(); }

  /**
   * @brief Get number of attributes in map.
   *
   * @return Number of attributes in map.
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
   * @brief Get all attribute names.
   *
   * @return List of all Attribute names.
   */
  std::vector<std::string> GetAttributeNames() const;

  /**
   * @brief Add attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   * @return true when successful.
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
   * @brief Clear all attributes.
   */
  void Clear();

  /**
   * @brief Remove attribute with given name.
   *
   * @param name Name of attribute to remove.
   * @return true when attribute was found and removed.
   */
  bool Remove(const std::string& name);

  /**
   * @brief Initialise variable attributes with values from other map.
   *
   * @param source Map containing variable name - value pairs.
   * @return true when all variable attributes were initialised.
   */
  bool InitialiseVariableAttributes(const AttributeMap& source);

private:
  map_type::iterator Find(const std::string& name);
  map_type::const_iterator Find(const std::string& name) const;
  map_type m_attributes;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_MAP_H_
