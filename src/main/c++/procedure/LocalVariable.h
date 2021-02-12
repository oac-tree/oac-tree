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
 * @file LocalVariable.h
 * @brief Header file for LocalVariable class.
 * @date 11/12/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the LocalVariable class.
 */

#ifndef _SEQ_LocalVariable_h_
#define _SEQ_LocalVariable_h_

// Global header files

// Local header files

#include "Variable.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Variable that is hosted locally in the workspace.
 */

class LocalVariable : public Variable
{
private:
  ::ccs::types::AnyValue _val;
  bool _initialized;

    /**
   * @brief Get value of locally hosted variable.
   *
   * @param value variable reference to contain the value.
   * @return true on success.
   */
  bool GetValueImpl(::ccs::types::AnyValue& value) const override;

  /**
   * @brief Set value of locally hosted variable.
   *
   * @param value value to set.
   * @return true on success.
   */
  bool SetValueImpl(const ::ccs::types::AnyValue& value) override;

protected:

public:
  // TODO: fix the double constructors (type parsing should be moved from VariableParser
  // to LocalVariable)
  LocalVariable();
  /**
   * @brief Constructor.
   *
   * @param type AnyType of underlying value.
   */
  LocalVariable(const ::ccs::base::SharedReference<const ccs::types::AnyType>& type);

  /**
   * @brief Destructor.
   */
  ~LocalVariable() override;

  static const std::string Type;
};

// Global variables

static const std::string LOCAL_VARIABLE_JSON_TYPE="type";
static const std::string LOCAL_VARIABLE_JSON_VALUE="value";

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

#endif // _SEQ_LocalVariable_h_
