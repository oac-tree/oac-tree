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
   * @brief Mutex for concurrent access of Variable.
   */
  mutable std::mutex _access_mutex;

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

protected:

public:
  /**
   * @brief Destructor.
   */
  virtual ~Variable();

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
