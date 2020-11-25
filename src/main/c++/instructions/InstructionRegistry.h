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
 * @file InstructionRegistry.h
 * @brief Header file for the InstructionRegistry class.
 * @date 20/11/2020
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the InstructionRegistry class.
 */

#ifndef _SEQ_InstructionRegistry_h_
#define _SEQ_InstructionRegistry_h_

// Global header files

#include <map>
#include <memory>
#include <string>
#include <vector>

// Local header files

// Constants

#ifdef __cplusplus

namespace sup {

namespace sequencer {

// Forward declarations

class Instruction;

// Type definition

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

// Global variables

// Function declarations

InstructionRegistry & GlobalInstructionRegistry();

bool InitGlobalInstructionRegistry();

// Function definitions

/**
 * @brief Template function for registering instructions.
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
bool RegisterInstruction()
{
  auto constructor = []() { return static_cast<Instruction*>(new T()); };
  return GlobalInstructionRegistry().RegisterInstruction(T::Type, constructor);
}

} // namespace sequencer

} // namespace sup

extern "C" {
#endif // __cplusplus

// C API function declarations

#ifdef __cplusplus
} // extern C
#endif // __cplusplus

#endif // _SEQ_InstructionRegistry_h_
