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

/**
 * @file TreeData.h
 * @brief Header file for TreeData class.
 * @date 01/02/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the TreeData class.
 */

#ifndef _SEQ_TreeData_h_
#define _SEQ_TreeData_h_

// Global header files

#include <string>
#include <utility>
#include <vector>

// Local header files

#include "AttributeMap.h"

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Type definition

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
    TreeData(const std::string & type);

    /**
     * @brief Destructor.
     */
    ~TreeData();

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
     * @brief Indicate presence of attribute with given name.
     *
     * @param name Attribute name.
     * @return true when present.
     */
    bool HasAttribute(const std::string & name) const;

    /**
     * @brief Get attribute with given name.
     *
     * @param name Attribute name.
     * @return Attribute value.
     */
    std::string GetAttribute(const std::string & name) const;

    /**
     * @brief Add attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return true on successful adding.
     */
    bool AddAttribute(const std::string & name, const std::string & value);

    /**
     * @brief Retrieve map of all attributes.
     *
     * @return Map of all attributes.
     */
    std::vector<std::pair<const std::string, std::string>> Attributes() const;

    /**
     * @brief Add child element data.
     *
     * @param child Data representation of child element.
     * @return true on success.
     */
    bool AddChild(const TreeData & child);

    /**
     * @brief Retrieve all child data elements.
     *
     * @return List of child data elements.
     */
    const std::vector<TreeData> & Children() const;

    /**
     * @brief Set element content string.
     *
     * @param content Content string.
     * @return true on success.
     */
    bool SetContent(std::string content);

    /**
     * @brief Retrieve element content string.
     *
     * @return Content string.
     */
    std::string GetContent() const;
};

// Global variables

static const std::string NAME_ATTRIBUTE = "name";

// Function declarations

// Function definitions

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_TreeData_h_
