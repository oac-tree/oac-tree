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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_INSTRUCTION_HELPER_H_
#define SUP_SEQUENCER_INSTRUCTION_HELPER_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
namespace InstructionHelper
{
/**
 * @brief Find an instruction by its pathname in a list of instructions.
 *
 * @param instructions List of instruction to search.
 * @param name_path Full path of names to reach requested instruction.
 *
 * @return The found instruction or nullptr.
 */
const Instruction* FindInstruction(const std::vector<const Instruction*>& instructions,
                                   const std::string& name_path);

/**
 * @brief Clone an instruction, including its descendents.
 *
 * @param instruction Instruction to clone.
 *
 * @return Clone of instruction tree or nullptr on failure.
 *
 * @details Caller needs to take ownership of the cloned instruction tree.
 */
Instruction* CloneInstruction(const Instruction* instruction);

/**
 * @brief Initialize variable attributes.
 *
 * @param instruction Instruction tree to initialise.
 * @param instructions List of attributes to use for initialisation.
 *
 * @return true on successful initialisation of variable attributes.
 */
bool InitialiseVariableAttributes(Instruction& instruction,
                                  const StringAttributeList& source_attributes);

/**
 * @brief Initialize variable attributes of an AttributeHandler.
 *
 * @param attribute_handler Attribute handler.
 * @param source_attributes List of attributes to use for initialisation.
 *
 * @return true on successful initialisation of variable attributes.
 */
bool InitialiseVariableAttributes(AttributeHandler& attribute_handler,
                                  const StringAttributeList& source_attributes);

}  // namespace InstructionHelper

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_HELPER_H_
