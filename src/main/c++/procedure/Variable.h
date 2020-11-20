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
 * @brief Header file for Variable class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Variable class.
 */

#ifndef _SEQ_Variable_h_
#define _SEQ_Variable_h_

// Global header files

#include <string>

// Local header files

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Class providing .
 */

class Variable
{
  private:
    std::string _name;
    int _value;

  protected:

  public:
    /**
     * @brief Constructor.
     *
     * @param name Variable name.
     */
    Variable(std::string name);

    /**
     * @brief Destructor.
     */
    ~Variable();

    /**
     * @brief Copy constructor.
     */
    Variable(const Variable & other);

    /**
     * @brief Get name of variable.
     */
    std::string GetName() const;

    /**
     * @brief Get value of variable.
     */
    int GetValue() const;

    /**
     * @brief Set value of variable.
     *
     * @param val Variable value
     */
    bool SetValue(int val);
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
