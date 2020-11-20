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
 * @file
 * @brief Header file for .
 * @date dd/MM/YYYY
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the _____ class.
 */

#ifndef _SEQ_Attribute_h_
#define _SEQ_Attribute_h_

// Global header files

#include <string>

// Local header files

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Class representing an attribute.
 */
class Attribute
{
  private:
    const std::string _name;
    const std::string _value;

  protected:

  public:
    /**
     * @brief Constructor.
     * @param name Attribute name.
     * @param value Attribute value.
     */
    Attribute(std::string name, std::string value);

    /**
     * @brief Destructor.
     */
    ~Attribute();

    /**
     * @brief Method.
     * @param name par.
     * @return Return value.
     */
    const std::string Name() const;

    /**
     * @brief Method.
     * @param name par.
     * @return Return value.
     */
    const std::string Value() const;
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

#endif // _SEQ_Attribute_h_
