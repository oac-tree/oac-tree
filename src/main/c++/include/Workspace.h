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
 * @file Workspace.h
 * @brief Header file for Workspace class.
 * @date 14/12/20
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the Workspace class.
 */

#ifndef _SEQ_Workspace_h_
#define _SEQ_Workspace_h_

// Global header files

#include <common/AnyValue.h>

#include <map>
#include <set>
#include <vector>

// Local header files

#include "Variable.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Container class for managing variables.
 */

class Workspace
{
private:
  /**
   * @brief Map from Variable names to Variable pointers.
   */
  std::map<std::string, Variable*> _var_map;

  /**
   * @brief Set of Variable pointers to check uniqueness and facilitate cleanup.
   */
  std::set<Variable*> _var_pointers;

protected:
public:
  /**
   * @brief Constructor.
   */
  Workspace();

  /**
   * @brief Destructor.
   */
  ~Workspace();

  /**
   * @brief Add variable.
   *
   * @param name Variable name.
   * @param var Variable to add.
   */
  bool AddVariable(std::string name, Variable* var);

  /**
   * @brief List all variable names.
   */
  std::vector<std::string> VariableNames() const;

  /**
   * @brief Get variable value.
   *
   * @param name Variable name.
   */
  bool GetValue(std::string name, ::ccs::types::AnyValue& value) const;

  /**
   * @brief Set variable value
   *
   * @param name Variable name.
   * @param name Variable value.
   */
  bool SetValue(std::string name, const ::ccs::types::AnyValue& value);

  std::vector<const Variable*> GetVariables() const;
};

// Global variables

// Function declarations

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_Workspace_h_
