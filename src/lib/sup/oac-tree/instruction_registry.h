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

#ifndef SUP_OAC_TREE_INSTRUCTION_REGISTRY_H_
#define SUP_OAC_TREE_INSTRUCTION_REGISTRY_H_

#include <sup/oac-tree/documentation_entry.h>
#include <sup/oac-tree/instruction.h>

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
 * @brief Registry holding a mapping between Instruction names and
 * their constructors.
 */

class InstructionRegistry
{
public:
  using InstructionConstructor = Instruction *(*)();

  InstructionRegistry() = default;

  // Copy/move construction/assignment
  InstructionRegistry(const InstructionRegistry& other) = default;
  InstructionRegistry(InstructionRegistry&& other) = delete;
  InstructionRegistry& operator=(const InstructionRegistry& other) = default;
  InstructionRegistry& operator=(InstructionRegistry&& other) = delete;

  /**
   * @brief Register an instruction.
   *
   * @param name Name under which the instruction is to be registered.
   * @param constructor Function that instantiates the instruction.
   * @param doc Documentation object for the instruction.
   */
  bool RegisterInstruction(std::string name, InstructionConstructor constructor,
                           DocumentationEntry doc);

  /**
   * @brief Instantiate an instruction with the given name.
   *
   * @param name Name under which the instruction was registered.
   * @throws InvalidOperationException when the instruction was not found.
   */
  std::unique_ptr<Instruction> Create(const std::string& name);

  /**
   * @brief Get documentation object for the given instruction.
   *
   * @param name Name under which the instruction was registered.
   * @throws InvalidOperationException when the instruction was not found.
   */
  DocumentationEntry GetDocumentation(const std::string& name) const;

  /**
   * @brief Set documentation object field for the given instruction.
   *
   * @param name Name under which the instruction was registered.
   * @param field_name Name of the field to set.
   * @param field_val Value of the field to set.
   * @throws InvalidOperationException when the instruction was not found.
   */
  void SetDocumentationField(const std::string& name, const std::string& field_name,
                             const std::string& field_val);

  /**
   * @brief List names of registered instructions.
   */
  std::vector<std::string> RegisteredInstructionNames() const;

  /**
   * @brief Check if instruction with the given name was registered.
   */
  bool IsRegisteredInstructionName(const std::string& name) const;

private:
  struct InstructionEntry
  {
    InstructionConstructor ctor;
    DocumentationEntry doc;
  };
  /**
   * @brief Map between instruction typename and an entry containing a ctor and documentation.
   */
  std::map<std::string, InstructionEntry> m_instruction_entries;
};

/**
 * @brief Set a documentation field of all instructions in a registry to a specific value if
 * their current value is empty (or does not exist).
 *
 * @param registry Registry whose instruction documentations to set.
 * @param field_name Field name of the documentation to set.
 * @param field_val Field value of the documentation to set.
 */
void SetAllEmptyDocFields(InstructionRegistry& registry, const std::string& field_name,
                          const std::string& field_val);

/**
 * @brief Get all plugin names found in the documentation of the instructions in the registry.
 *
 * @note 'CORE' and empty plugin names are filtered out.
 *
 * @param registry Registry to query.
 * @return List of plugin names.
 */
std::set<std::string> GetAllPluginNames(const InstructionRegistry& registry);

/**
 * @brief Get all instructions from the provided plugin (must be loaded).
 *
 * @note Use 'CORE' as plugin name to get all core instructions.
 * @note Use empty plugin_name for all instructions that have no registered plugin name.
 *
 * @param registry Registry to query.
 * @param plugin_name Plugin name as it is registered in the registry.
 * @return List of instruction names belonging to the plugin.
 */
std::vector<std::string> GetInstructionsFromPlugin(const InstructionRegistry& registry,
                                                   const std::string& plugin_name);

/**
 * @brief Get the plugin name under which an instruction is registered.
 *
 * @param registry Registry to query.
 * @param instr_name Instruction name.
 * @return Plugin name (if instruction from plugin), `CORE` (for core instructions) or empty
 * (if instruction does not exist).
 */
std::string GetPluginForInstruction(const InstructionRegistry& registry,
                                    const std::string& instr_name);

InstructionRegistry &GlobalInstructionRegistry();

/**
 * @brief Template function for registering instructions to a registry.
 *
 * @param registry Registry where to register the instruction.
 * @param doc Documentation object for the instruction.
 *
 * @note The registered instruction template parameter T needs to
 * define its typename as a static std::string member 'Type'
 *
 * @code
   // Declare Instruction with static 'Type' string
   class MyInstruction : public Instruction
   {
     ...
     static const std::string Type;
   };

   // Create a registry
   InstructionRegistry my_instruction_registry

   // Define the typename for the instruction
   const std::string MyInstruction::Type = "MyInstruction";

   // Define a documentation object
   DocumentationEntry my_doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Description for this instruction"}
   }};

   // Register the instruction
   static bool _MyInstructionRegistered =
     RegisterInstruction<MyInstruction>(my_instruction_registry, my_doc);
   @endcode
 */
template <class T>
bool RegisterInstruction(InstructionRegistry& registry, DocumentationEntry doc)
{
  auto constructor = []() { return static_cast<Instruction *>(new T()); };
  return registry.RegisterInstruction(T::Type, constructor, std::move(doc));
}

/**
 * @brief Template function for registering instructions to a registry.
 *
 * @param registry Registry where to register the instruction.
 *
 * @note This function will use the template function InstructionDocFor<T> to get the documentation
 * object that will be registered. If that function template is not specialized for the instruction
 * type, this will generate an empty default documentation object.
 */
template <class T>
bool RegisterInstruction(InstructionRegistry& registry)
{
  auto constructor = []() { return static_cast<Instruction *>(new T()); };
  const auto& doc = InstructionDocFor<T>();
  return registry.RegisterInstruction(T::Type, constructor, doc);
}

/**
 * @brief Template function for registering instructions to the global registry.
 *
 * @param doc Documentation object for the instruction.
 *
 * @note The registered instruction template parameter T needs to
 * define its typename as a static std::string member 'Type'
 *
 * @code
   // Declare Instruction with static 'Type' string
   class MyInstruction : public Instruction
   {
     ...
     static const std::string Type;
   };

   // Define the typename for the instruction
   const std::string MyInstruction::Type = "MyInstruction";

   // Register the instruction for serialization
   static bool _MyInstructionRegistered = RegisterInstruction<MyInstruction>();
   @endcode
 */
template <class T>
bool RegisterGlobalInstruction(DocumentationEntry doc)
{
  return RegisterInstruction<T>(GlobalInstructionRegistry(), std::move(doc));
}

/**
 * @brief Template function for registering instructions to the global registry.
 *
 * @note This function will use the template function InstructionDocFor<T> to get the documentation
 * object that will be registered. If that function template is not specialized for the instruction
 * type, this will generate an empty default documentation object.
 */
template <class T>
bool RegisterGlobalInstruction()
{
  return RegisterInstruction<T>(GlobalInstructionRegistry());
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_REGISTRY_H_
