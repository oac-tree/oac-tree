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

#ifndef SUP_OAC_TREE_INSTRUCTION_HELPER_H_
#define SUP_OAC_TREE_INSTRUCTION_HELPER_H_

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
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
 * @return Clone of instruction tree or invalid unique pointer on failure.
 */
std::unique_ptr<Instruction> CloneInstruction(const Instruction* instruction);

/**
 * @brief Initialize variable attributes.
 *
 * @param instruction Instruction tree to initialise.
 * @param source_attributes List of attributes to use for initialisation.
 *
 * @return true on successful initialisation of variable attributes.
 */
bool InitialisePlaceholderAttributes(Instruction& instruction,
                                  const StringAttributeList& source_attributes);

/**
 * @brief Initialize variable attributes of an AttributeHandler.
 *
 * @param attribute_handler Attribute handler.
 * @param source_attributes List of attributes to use for initialisation.
 *
 * @return true on successful initialisation of variable attributes.
 */
bool InitialisePlaceholderAttributes(AttributeHandler& attribute_handler,
                                  const StringAttributeList& source_attributes);

/**
 * @brief Check if attribute string starts with a given character.
 *
 * @param attr_str Attribute string.
 * @param c Character to check.
 *
 * @return true when the attribute string starts with the given character, false otherwise.
 */
bool AttributeStartsWith(const std::string& attr_str, char c);

}  // namespace InstructionHelper

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_HELPER_H_
