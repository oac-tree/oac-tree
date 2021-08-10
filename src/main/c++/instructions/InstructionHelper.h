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
 * @file InstructionHelper.h
 * @brief Header file for Instruction helper functions.
 * @date 03/03/2021
 * @author Walter Van Herck (IO)
 * @copyright 2010-2021 ITER Organization
 * @details This header file contains the definition of the Instruction helper functions.
 */

#ifndef _SEQ_InstructionHelper_h_
#define _SEQ_InstructionHelper_h_

// Global header files

// Local header files

#include "Instruction.h"

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
namespace InstructionHelper
{
// Global variables

// Function declarations

/**
 * @brief Find an instruction by its pathname in a list of instructions.
 *
 * @param instructions List of instruction to search.
 * @param name_path Full path of names to reach requested instruction.
 * @return The found instruction or nullptr.
 */
const Instruction *FindInstruction(const std::vector<const Instruction *> &instructions,
                                   const std::string &name_path);

/**
 * @brief Clone an instruction, including its descendents.
 *
 * @param instruction Instruction to clone.
 * @return Clone of instruction tree.
 * @details Caller needs to take ownership of the cloned instruction tree.
 */
Instruction *CloneInstruction(const Instruction *instruction);

/**
 * @brief Initialize variable attributes.
 *
 * @param instruction Instruction tree to initialise.
 * @param instructions List of attributes to use for initialisation.
 * @return true on successful initialisation of variable attributes.
 */
bool InitialiseVariableAttributes(Instruction &instruction, const AttributeMap &attributes);

// Function definitions

}  // namespace InstructionHelper

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_InstructionHelper_h_
