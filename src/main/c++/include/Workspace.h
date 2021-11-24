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

#include <map>
#include <memory>
#include <vector>

#include <common/AnyValue.h>

#include "Variable.h"
#include "CallBackManager.h"

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
   *
   * @note Workspace owns its Variable objects.
   */
  std::map<std::string, std::unique_ptr<Variable>> _var_map;

  /**
   * @brief Threadsafe list of callback objects.
   */
  CallbackManager<void(const std::string&, const ccs::types::AnyValue&)> callbacks;

  /**
   * @brief Check if the given Variable name is already present.
   *
   * @param name Name to check.
   * @return true if Variable name is already present.
   */
  bool ContainsVariableName(const std::string& name) const;

  /**
   * @brief Check if the Variable pointer is already present.
   *
   * @param var Variable pointer to check.
   * @return true if Variable pointer is already present.
   */
  bool ContainsVariablePointer(Variable* var) const;

  /**
   * @brief Method which is called if a variable is updated.
   *
   * @param name Variable name.
   * @param name Variable's new value.
   */
  void VariableUpdated(const std::string name, const ccs::types::AnyValue& value);

public:
  Workspace();
  ~Workspace();

  /**
   * @brief Add variable.
   *
   * @param name Variable name.
   * @param var Variable pointer.
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

  /**
   * @brief Get variable with given name
   *
   * @param name Variable name.
   * @return const pointer to found variable or null pointer otherwise.
   */
  const Variable* GetVariable(const std::string& name) const;

  /**
   * @brief Check existence of variable with given name
   *
   * @param name Variable name.
   * @return true if variable exists.
   */
  bool HasVariable(const std::string& name) const;

  /**
   * @brief Add callback for variable updates
   *
   * @param cb Callback function object.
   * @return true if adding the callback was successful.
   */
  bool AddUpdateCallback(
    const std::function<void(const std::string&, const ccs::types::AnyValue&)>& cb);
};

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_Workspace_h_
