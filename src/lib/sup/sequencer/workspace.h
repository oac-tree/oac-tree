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
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <map>
#include <memory>
#include <vector>

namespace sup
{
namespace dto
{
class AnyValue;
class AnyTypeRegistry;
}  // namespace dto

namespace sequencer
{
/**
 * @brief Container class for managing variables.
 */
class Workspace
{
public:
  using GenericCallback = std::function<void(const std::string&, const sup::dto::AnyValue&, bool)>;
  using VariableCallback = std::function<void(const sup::dto::AnyValue&, bool)>;

  Workspace(const std::string& filename = "");
  ~Workspace();

  /**
   * @brief Get the filename of the owning procedure (if loaded from file).
   *
   * @return Filename of this workspace's procedure.
   * @details This filename can be used to resolve relative pathnames.
   */
  std::string GetFilename() const;

  /**
   * @brief Add variable.
   *
   * @param name Variable name.
   * @param var Variable to add.
   */
  bool AddVariable(const std::string& name, std::unique_ptr<Variable>&& var);

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
  void Teardown();

  /**
   * @brief Reset specific variable.
   *
   * @param varname Variable name.
   * @return True if variable was found and reset.
   */
  bool ResetVariable(const std::string& varname);

  /**
   * @brief Get variable value.
   *
   * @param name Variable name/field.
   * @param value AnyValue output parameter.
   *
   * @return True on success.
   */
  bool GetValue(const std::string& name, sup::dto::AnyValue& value) const;

  /**
   * @brief Set variable value
   *
   * @param name Variable name/field.
   * @param value Variable value.
   *
   * @return True on success.
   */
  bool SetValue(const std::string& name, const sup::dto::AnyValue& value);

  /**
   * @brief Wait with timeout for variable to become available.
   *
   * @param name Variable name.
   * @param timeout_sec Timeout in seconds.
   * @param availability If true, wait for variable to become available, otherwise wait for
   * inavailability.
   *
   * @return True if not timed out.
   */
  bool WaitForVariable(const std::string& name, double timeout_sec, bool availability = true);

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
   * @brief Register an AnyType instance under its own name.
   *
   * @param anytype AnyType instance to register.
   *
   * @return true on successful registration.
   */
  bool RegisterType(const sup::dto::AnyType& anytype);

  /**
   * @brief Get the current type registry.
   *
   * @return Reference to type registry.
   */
  const sup::dto::AnyTypeRegistry& GetTypeRegistry() const;

  /**
   * @brief Get a callback guard object.
   *
   * @param listener Pointer to object that listens to updates.
   * @return Guard object.
   *
   * @note Upon destruction, the guard object will unregister its callbacks. Users are responsible
   * for creating such an object before registering callbacks as there is no other way to unregister
   * them. Note also that unregistering the callbacks should only be done when the object the
   * callbacks refer to (e.g. for member functions) is destroyed before the Workspace.
   */
  ScopeGuard GetCallbackGuard(void* listener);

  /**
   * @brief Add callback for variable updates
   *
   * @param cb Callback function object.
   * @param listener Pointer to object that listens to these updates (used for unregistering).
   * @return true if adding the callback was successful.
   *
   * @note Generic callbacks will be called for each variable update in the workspace.
   * @note If a pointer to the listening object is not provided (=nullptr), the callbacks are
   * required to outlive the Workspace.
   */
  bool RegisterGenericCallback(const GenericCallback& cb, void* listener = nullptr);

  /**
   * @brief Add callback for a specific variable update.
   *
   * @param name Variable name.
   * @param cb Callback function object.
   * @param listener Pointer to object that listens to these updates (used for unregistering).
   * @return true if adding the callback was successful.
   */
  bool RegisterCallback(const std::string& name, const VariableCallback& cb, void* listener);

  /**
   * @brief Query if the workspace was already successfully setup.
   *
   * @note Adding variables after a successful setup requires resetting the workspace and calling
   * Setup() again.
   */
  bool IsSuccessfullySetup() const;

private:
  /**
   * @brief Filename of the Procedure it's part of.
   */
  std::string m_filename;

  /**
   * @brief Map from Variable names to Variable pointers.
   *
   * @note Workspace owns its Variable objects.
   */
  std::map<std::string, std::unique_ptr<Variable>> m_var_map;

  /**
   * @brief List of variable names to allow to retrieve variables in the order they were added.
   */
  std::vector<std::string> m_var_names;

  /**
   * @brief Threadsafe list of callback objects.
   */
  NamedCallbackManager<const sup::dto::AnyValue&, bool> m_callbacks;

  std::unique_ptr<sup::dto::AnyTypeRegistry> m_type_registry;

  std::vector<std::function<void()>> m_teardown_actions;

  /**
   * @brief Flag that indicates setup was already called on this workspace.
   */
  bool m_setup_done;

  /**
   * @brief Check if the given Variable name is already present.
   *
   * @param name Name to check.
   * @return true if Variable name is already present.
   */
  bool ContainsVariableName(const std::string& name) const;

  /**
   * @brief Store unique teardown actions and return unique setup actions.
   *
   * @param actions list of SetupTeardownActions.
   * @return List of unique (according to identifier) setup actions.
   */
  std::vector<std::function<void()>> ParseSetupTeardownActions(
    const std::vector<SetupTeardownActions>& actions);

  /**
   * @brief Method which is called if a variable is updated.
   *
   * @param name Variable name.
   * @param value Variable's new value.
   * @param connected New availibility status.
   */
  void VariableUpdated(const std::string name, const sup::dto::AnyValue& value,
                       bool connected) const;
};

std::pair<std::string, std::string> SplitFieldName(const std::string &fullname);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_WORKSPACE_H_
