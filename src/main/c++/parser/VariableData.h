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
 * @file VariableData.h
 * @brief Header file for VariableData class.
 * @date 14/12/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the VariableData class.
 */

#ifndef _SEQ_VariableData_h_
#define _SEQ_VariableData_h_

// Global header files

#include <map>
#include <memory>
#include <string>

// Local header files

#include "procedure/Variable.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup {

namespace sequencer {

/**
 * @brief Data representation of an Instruction.
 */

class VariableData
{
private:
  std::string _type;
  std::map<std::string, std::string> _attributes;

protected:

public:
  /**
   * @brief Constructor.
   * @param var_type Variable typename.
   */
  VariableData(std::string var_type);

  /**
   * @brief Destructor.
   */
  ~VariableData() = default;

  /**
   * @brief Retrieve the variable's typename.
   *
   * @return Typename of variable.
   */
  std::string GetType() const;

  /**
   * @brief Retrieve the variable's name attribute.
   *
   * @return Name of variable.
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
  const std::map<std::string, std::string> & Attributes() const;

  /**
   * @brief Generate the Variable object corresponding to the current data.
   *
   * @return Unique pointer to Variable object.
   */
  std::unique_ptr<Variable> GenerateVariable() const;
};

// Global variables

static const std::string VARIABLE_NAME_ATTRIBUTE = "name";

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

#endif // _SEQ_VariableData_h_
