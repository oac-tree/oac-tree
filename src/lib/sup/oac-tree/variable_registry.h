/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_VARIABLE_REGISTRY_H_
#define SUP_OAC_TREE_VARIABLE_REGISTRY_H_

#include <sup/oac-tree/documentation_entry.h>
#include <sup/oac-tree/variable.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Registry holding a mapping between Variable names and
 * their constructors.
 */
class VariableRegistry
{
public:
  using VariableConstructor = Variable *(*)();

  VariableRegistry() = default;

  // Copy/move construction/assignment
  VariableRegistry(const VariableRegistry& other) = default;
  VariableRegistry(VariableRegistry&& other) = delete;
  VariableRegistry& operator=(const VariableRegistry& other) = default;
  VariableRegistry& operator=(VariableRegistry&& other) = delete;

  /**
   * @brief Register a variable.
   *
   * @param name Name under which the variable is to be registered.
   * @param constructor Function that instantiates the variable.
   * @param doc Documentation object for the variable.
   */
  bool RegisterVariable(std::string name, VariableConstructor constructor,
                        DocumentationEntry doc);

  /**
   * @brief Instantiate a variable with the given name.
   *
   * @param name Name under which the variable was registered.
   */
  std::unique_ptr<Variable> Create(std::string name);

  /**
   * @brief Get documentation object for the given variable.
   *
   * @param name Name under which the variable was registered.
   * @throws InvalidOperationException when the variable was not found.
   */
  DocumentationEntry GetDocumentation(const std::string& name) const;

  /**
   * @brief Set documentation object field for the given variable.
   *
   * @param name Name under which the variable was registered.
   * @param field_name Name of the field to set.
   * @param field_val Value of the field to set.
   * @throws InvalidOperationException when the variable was not found.
   */
  void SetDocumentationField(const std::string& name, const std::string& field_name,
                             const std::string& field_val);

  /**
   * @brief List names of registered variables.
   */
  std::vector<std::string> RegisteredVariableNames() const;

  /**
   * @brief Check if variable with the given name was registered.
   */
  bool IsRegisteredVariableName(const std::string& name) const;

private:
  struct VariableEntry
  {
    VariableConstructor ctor;
    DocumentationEntry doc;
  };
  /**
   * @brief Map between Variable typename and an entry containing a ctor and documentation.
   */
  std::map<std::string, VariableEntry> m_variable_entries;
};

/**
 * @brief Set a documentation field of all variables in a registry to a specific value if
 * their current value is empty (or does not exist).
 *
 * @param registry Registry whose variable documentations to set.
 * @param field_name Field name of the documentation to set.
 * @param field_val Field value of the documentation to set.
 */
void SetAllEmptyDocFields(VariableRegistry& registry, const std::string& field_name,
                          const std::string& field_val);

/**
 * @brief Get all plugin names found in the documentation of the variables in the registry.
 *
 * @note 'CORE' and empty plugin names are filtered out.
 *
 * @param registry Registry to query.
 * @return List of plugin names.
 */
std::set<std::string> GetAllPluginNames(const VariableRegistry& registry);

/**
 * @brief Get all variables from the provided plugin (must be loaded).
 *
 * @note Use 'CORE' as plugin name to get all core variables.
 * @note Use empty plugin_name for all variables that have no registered plugin name.
 *
 * @param registry Registry to query.
 * @param plugin_name Plugin name as it is registered in the registry.
 * @return List of variable names belonging to the plugin.
 */
std::vector<std::string> GetVariablesFromPlugin(const VariableRegistry& registry,
                                                const std::string& plugin_name);

/**
 * @brief Get the plugin name under which a variable is registered.
 *
 * @param registry Registry to query.
 * @param var_name Variable name.
 * @return Plugin name (if variable from plugin), `CORE` (for core variables) or empty
 * (if variable does not exist).
 */
std::string GetPluginForVariable(const VariableRegistry& registry, const std::string& var_name);

VariableRegistry &GlobalVariableRegistry();

/**
 * @brief Template function for registering variables to a registry.
 *
 * @param registry Registry where to register the variable.
 * @param doc Documentation object for the variable.
 *
 * @note The registered variable template parameter T needs to
 * define its typename as a static std::string member 'Type'
 *
 * @code
   // Declare Variable with static 'Type' string
   class MyVariable : public Variable
   {
     ...
     static const std::string Type;
   };

   // Create a registry
   VariableRegistry my_variable_registry

   // Define the typename for the variable
   const std::string MyVariable::Type = "MyVariable";

   // Define a documentation object
   DocumentationEntry my_doc{{
    { Constants::VAR_DOC_DESCRIPTION, "Description for this variable"}
   }};

   // Register the variable
   static bool _MyVariableRegistered =
     RegisterVariable<MyVariable>(my_variable_registry, my_doc);
   @endcode
 */
template <class T>
bool RegisterVariable(VariableRegistry& registry, DocumentationEntry doc)
{
  auto constructor = []() { return static_cast<Variable *>(new T()); };
  return registry.RegisterVariable(T::Type, constructor, std::move(doc));
}

/**
 * @brief Template function for registering variables to a registry.
 *
 * @param registry Registry where to register the variable.
 *
 * @note This function will use the template function VariableDocFor<T> to get the documentation
 * object that will be registered. If that function template is not specialized for the variable
 * type, this will generate an empty default documentation object.
 */
template <class T>
bool RegisterVariable(VariableRegistry& registry)
{
  auto constructor = []() { return static_cast<Variable *>(new T()); };
  const auto& doc = VariableDocFor<T>();
  return registry.RegisterVariable(T::Type, constructor, doc);
}

/**
 * @brief Template function for registering variables to the global registry.
 *
 * @param doc Documentation object for the variable.
 *
 * @note The registered variable template parameter T needs to
 * define its typename as a static std::string member 'Type'
 *
 * @code
   // Declare Variable with static 'Type' string
   class MyVariable : public Variable
   {
     ...
     static const std::string Type;
   };

   // Define the typename for the variable
   const std::string MyVariable::Type = "MyVariable";

   // Register the variable for serialization
   static bool _MyVariableRegistered = RegisterVariable<MyVariable>();
   @endcode
 */
template <class T>
bool RegisterGlobalVariable(DocumentationEntry doc)
{
  return RegisterVariable<T>(GlobalVariableRegistry(), std::move(doc));
}

/**
 * @brief Template function for registering variables to the global registry.
 *
 * @note This function will use the template function VariableDocFor<T> to get the documentation
 * object that will be registered. If that function template is not specialized for the variable
 * type, this will generate an empty default documentation object.
 */
template <class T>
bool RegisterGlobalVariable()
{
  return RegisterVariable<T>(GlobalVariableRegistry());
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_VARIABLE_REGISTRY_H_
