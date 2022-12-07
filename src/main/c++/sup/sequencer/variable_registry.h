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

#ifndef SUP_SEQUENCER_VARIABLE_REGISTRY_H_
#define SUP_SEQUENCER_VARIABLE_REGISTRY_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
class Variable;

/**
 * @brief Registry holding a mapping between Variable names and
 * their constructors.
 */
class VariableRegistry
{
public:
  using VariableConstructor = Variable *(*)();

  VariableRegistry() = default;

  /**
   * @brief Register a variable.
   *
   * @param name Name under which the variable is to be registered.
   * @param constructor Function that instantiates the variable.
   */
  bool RegisterVariable(std::string name, VariableConstructor constructor);

  /**
   * @brief Instantiate a variable with the given name.
   *
   * @param name Name under which the variable was registered.
   */
  std::unique_ptr<Variable> Create(std::string name);

  /**
   * @brief List names of registered variables.
   */
  std::vector<std::string> RegisteredVariableNames() const;

  /**
   * @brief Check if variable with the given name was registered.
   */
  bool IsRegisteredVariableName(const std::string& name) const;

private:
  /**
   * @brief Map between Variable typename and its constructor.
   */
  std::map<std::string, VariableConstructor> m_variable_map;
};

VariableRegistry &GlobalVariableRegistry();

/**
 * @brief Template function for registering variables to a registry.
 *
 * @param registry Registry where to register the variable.
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

   // Register the variable
   static bool _MyVariableRegistered =
     RegisterVariable<MyVariable>(my_variable_registry);
   @endcode
 */
template <class T>
bool RegisterVariable(VariableRegistry &registry)
{
  auto constructor = []() { return static_cast<Variable *>(new T()); };
  return registry.RegisterVariable(T::Type, constructor);
}

/**
 * @brief Template function for registering variables to the global registry.
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
bool RegisterGlobalVariable()
{
  return RegisterVariable<T>(GlobalVariableRegistry());
}

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_VARIABLE_REGISTRY_H_
