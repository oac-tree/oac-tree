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
 * @file AttributeMap.h
 * @brief Header file for AttributeMap class.
 * @date 09/02/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the AttributeMap class.
 */

#ifndef _SEQ_AttributeMap_h_
#define _SEQ_AttributeMap_h_

// Global header files

#include <map>
#include <string>
#include <vector>

// Local header files

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Class that contains attributes and exposes a map interface.
 */

class AttributeMap
{
  private:
    /**
     * @brief Implementation of the map of attributes.
     */
    std::map<std::string, std::string> _attributes;

  protected:

  public:
    /**
     * @brief Constructor.
     */
    AttributeMap();

    /**
     * @brief Destructor.
     */
    ~AttributeMap();

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
     */
    bool AddAttribute(const std::string & name, const std::string & value);

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
    bool Remove(const std::string & name);
};

// Global variables

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

#endif // _SEQ_AttributeMap_h_
