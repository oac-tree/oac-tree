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

#ifndef SUP_SEQUENCER_INSTRUCTION_REGISTRY_H_
#define SUP_SEQUENCER_INSTRUCTION_REGISTRY_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
class Instruction;

/**
 * @brief Registry holding a mapping between Instruction names and
 * their constructors.
 */

class InstructionRegistry
{
private:
  /**
   * @brief Definition of Instruction constructor.
   */
  using InstructionConstructor = Instruction *(*)();

  /**
   * @brief Map between instruction typename and its constructor.
   */
  std::map<std::string, InstructionConstructor> _instruction_map;

protected:
public:
  /**
   * @brief Constructor.
   */
  InstructionRegistry() = default;

  /**
   * @brief Register an instruction.
   *
   * @param name Name under which the instruction is to be registered.
   * @param constructor Function that instantiates the instruction.
   */
  bool RegisterInstruction(std::string name, InstructionConstructor constructor);

  /**
   * @brief Instantiate an instruction with the given name.
   *
   * @param name Name under which the instruction was registered.
   */
  std::unique_ptr<Instruction> Create(std::string name);

  /**
   * @brief List names of registered instructions.
   */
  std::vector<std::string> RegisteredInstructionNames() const;
};

InstructionRegistry &GlobalInstructionRegistry();

/**
 * @brief Template function for registering instructions to a registry.
 *
 * @param registry Registry where to register the instruction.
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

   // Register the instruction
   static bool _MyInstructionRegistered =
     RegisterInstruction<MyInstruction>(my_instruction_registry);
   @endcode
 */
template <class T>
bool RegisterInstruction(InstructionRegistry &registry)
{
  auto constructor = []() { return static_cast<Instruction *>(new T()); };
  return registry.RegisterInstruction(T::Type, constructor);
}

/**
 * @brief Template function for registering instructions to the global registry.
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
bool RegisterGlobalInstruction()
{
  return RegisterInstruction<T>(GlobalInstructionRegistry());
}

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_REGISTRY_H_
