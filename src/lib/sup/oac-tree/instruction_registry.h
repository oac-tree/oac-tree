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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
class Instruction;

/**
 * @brief Registry holding a mapping between Instruction names and
 * their constructors.
 */

class InstructionRegistry
{
public:
  using InstructionConstructor = Instruction *(*)();

  InstructionRegistry() = default;

  /**
   * @brief Register an instruction.
   *
   * @param name Name under which the instruction is to be registered.
   * @param constructor Function that instantiates the instruction.
   * @param doc Documentation object for the instruction.
   */
  bool RegisterInstruction(std::string name, InstructionConstructor constructor,
                           DocomentationEntry doc);

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
  DocomentationEntry GetDocumentation(const std::string& name);

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
    DocomentationEntry doc;
  };
  /**
   * @brief Map between instruction typename and its constructor.
   */
  std::map<std::string, InstructionEntry> m_instruction_entries;
};

InstructionRegistry &GlobalInstructionRegistry();

/**
 * @brief Template function returning a empty DocomentationEntry. This needs to be specialized for
 * each instruction requiring a custom DocomentationEntry.
 *
 * @tparam T Instruction type parameter.
 * @return DocomentationEntry corresponding to the instruction type.
 */
template <typename T>
DocomentationEntry InstructionDocFor()
{
  return DocomentationEntry{};
}

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
   DocomentationEntry my_doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Description for this instruction"}
   }};

   // Register the instruction
   static bool _MyInstructionRegistered =
     RegisterInstruction<MyInstruction>(my_instruction_registry, my_doc);
   @endcode
 */
template <class T>
bool RegisterInstruction(InstructionRegistry& registry, DocomentationEntry doc)
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
bool RegisterGlobalInstruction(DocomentationEntry doc)
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
