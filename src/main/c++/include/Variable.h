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
 * @file Variable.h
 * @brief Header file for Variable interface.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Variable interface.
 */

#ifndef _SEQ_Variable_h_
#define _SEQ_Variable_h_

// Global header files

#include <mutex>
#include <common/AnyValue.h>

// Local header files

#include "AttributeMap.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Interface for workspace variables.
 */

class Variable
{
  private:
    /**
     * @brief Typename of this variable
     */
    std::string _type;

    /**
     * @brief Mutex for concurrent access of Variable.
     */
    mutable std::mutex _access_mutex;

    /**
     * @brief List of attributes.
     */
    AttributeMap _attributes;

    /**
     * @brief Indicates if the Variable was correctly setup from its attributes.
     */
    bool _setup_successful;

    /**
     * @brief Get value of variable.
     *
     * @param value variable reference to contain the value.
     * @return true on success.
     *
     * @note Private virtual implementation.
     */
    virtual bool GetValueImpl(::ccs::types::AnyValue& value) const =0;

    /**
     * @brief Set value of variable.
     *
     * @param value value to set.
     * @return true on success.
     *
     * @note Private virtual implementation.
     */
    virtual bool SetValueImpl(const ::ccs::types::AnyValue& value) =0;

    /**
     * @brief Setup value of variable.
     *
     * @note Private virtual implementation.
     */
    virtual bool SetupImpl();

  protected:

  public:
    /**
     * @brief Constructor.
     */    
    Variable(std::string type);

    /**
     * @brief Destructor.
     */
    virtual ~Variable();

    /**
     * @brief Get variable type
     * @return variable type
     */
    std::string GetType() const;

    /**
     * @brief Get variable name
     * @return variable name
     */
    std::string GetName() const;

    /**
     * @brief Set variable name
     * @param name Name to set
     * @return void
     */
    void SetName(std::string name);

    /**
     * @brief Get value of variable.
     *
     * @param value variable reference to contain the value.
     * @return true on success.
     *
     * @note Non-virtual interface.
     */
    bool GetValue(::ccs::types::AnyValue& value) const;

    /**
     * @brief Set value of variable.
     *
     * @param value value to set.
     * @return true on success.
     *
     * @note Non-virtual interface.
     */
    bool SetValue(const ::ccs::types::AnyValue& value);

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
     * @brief Set attribute with given name and value.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return true when successful.
     */
    bool AddAttribute(const std::string & name, const std::string & value);

    /**
     * @brief Set all attributes in given list.
     *
     * @param attributes Attribute list.
     * @return true when successful.
     */
    bool AddAttributes(const std::vector<std::pair<const std::string, std::string>> & attributes);

    /**
     * @brief Setup the variable with the currently present attributes.
     *
     * @return true on success.
     *
     * @note May do nothing if not all required attributes are present (yet).
     */
    bool Setup();
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

#endif // _SEQ_Variable_h_
