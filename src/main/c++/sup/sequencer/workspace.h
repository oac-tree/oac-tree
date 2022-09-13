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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_WORKSPACE_H_
#define SUP_SEQUENCER_WORKSPACE_H_

#include "named_callback_manager.h"
#include "variable.h"

#include <common/AnyValue.h>

#include <map>
#include <memory>
#include <vector>

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
  NamedCallbackManager<const ccs::types::AnyValue&> callbacks;

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
   * @brief Setup all variables.
   */
  void Setup();

  /**
   * @brief Teardown all variables.
   */
  void Reset();

  /**
   * @brief Reset specific variable.
   *
   * @param name Variable name.
   * @return True if variable was found and reset.
   */
  bool ResetVariable(const std::string& varname);

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
   * @brief Get a callback guard object.
   *
   * @param listener Pointer to object that listens to updates.
   * @return Guard object.
   *
   * @note Upon destruction, the guard object will unregister its callbacks. Users are responsible
   * for creating such an object before registering callback as there is no other way to unregister
   * them.
   */
  CallbackGuard<NamedCallbackManager<const ccs::types::AnyValue&>> GetCallbackGuard(void* listener);

  /**
   * @brief Add callback for variable updates
   *
   * @param cb Callback function object.
   * @return true if adding the callback was successful.
   *
   * @note Generic callbacks will be called for each variable update in the workspace.
   * @note If a pointer to the listening object is not provide (=nullptr), the callbacks are
   * required to outlive the Workspace.
   */
  bool RegisterGenericCallback(
      const std::function<void(const std::string&, const ccs::types::AnyValue&)>& cb,
      void* listener = nullptr);

  /**
   * @brief Add callback for a specific variable update.
   *
   * @param name Variable name.
   * @param cb Callback function object.
   * @param listener Pointer to object that listens to these updates (used for unregistering).
   * @return true if adding the callback was successful.
   *
   * @note The pointer is only used as an id to allow for later removal (unregister).
   */
  bool RegisterCallback(const std::string& name,
                        const std::function<void(const ccs::types::AnyValue&)>& cb, void* listener);
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_WORKSPACE_H_
