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

#ifndef SUP_SEQUENCER_INSTRUCTION_INFO_UTILS_H_
#define SUP_SEQUENCER_INSTRUCTION_INFO_UTILS_H_

#include <sup/sequencer/instruction_info.h>

#include <sup/dto/anyvalue.h>

#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
class Instruction;
class InstructionMap;

namespace utils
{
// TODO: remove the instr_map as parameter? This can be created directly from the root instruction.
/**
 * @brief Create a InstructionInfo representation of an Instruction tree, providing all Instruction
 * types, their attributes and indices.
 *
 * @param root Root instruction of the tree to represent.
 * @param instr_map Map of instructions to indices.
 *
 * @return InstructionInfo tree representation.
 * @throw InvalidOperationException when the provided tree does not correspond to the mapped
 * Instructions in the provided map.
 */
std::unique_ptr<InstructionInfo> CreateInstructionInfoTree(const Instruction& root,
                                                           const InstructionMap& instr_map);

/**
 * @brief Convert the given AnyValue to an InstructionInfo tree.
 *
 * @param instr_info_anyvalue AnyValue representation of an instruction tree.
 *
 * @return InstructionInfo tree.
 * @throw InvalidOperationException when the provided anyvalue has the wrong format or the resulting
 * InstructionInfo tree is inconsistent (null pointers or inconsistent indices).
 */
std::unique_ptr<InstructionInfo> ToInstructionInfoTree(
  const sup::dto::AnyValue& instr_info_anyvalue);

/**
 * @brief Convert the given InstructionInfo tree to an AnyValue.
 *
 * @param instr_info InstructionInfo tree.
 *
 * @return AnyValue representation of the InstructionInfo tree.
 */
sup::dto::AnyValue ToAnyValueTree(const InstructionInfo& instr_info);

/**
 * @brief Build a list of InstructionInfo pointers, ordered by index. This allows O(1) lookup.
 * It is assumed the indices are unique and span exactly the range from zero to
 * (number_of_instructions -1).
 *
 * @param instr_info InstructionInfo tree.
 *
 * @return Ordered list of InstructionInfo pointers.
 * @throw InvalidOperationException when the provided InstructionInfo tree has inconsistent
 * indices, i.e. duplicate indices or out of bound.
 */
std::vector<const InstructionInfo*> CreateOrderedInstructionInfo(const InstructionInfo& instr_info);

/**
 * @brief Create a InstructionInfo representation of a single Instruction, providing its type,
 * attributes and the given index. This function will not encode the Instruction's child
 * Instructions.
 *
 * @param instr Instruction to represent.
 * @param index Index to use.
 *
 * @return InstructionInfo representation.
 */
std::unique_ptr<InstructionInfo> CreateInstructionInfoNode(const Instruction& instr,
                                                           sup::dto::uint32 index);

/**
 * @brief Convert the given AnyValue to a single InstructionInfo node. This function ignores any
 * possible child instructions.
 *
 * @param instr_info_anyvalue AnyValue representation of an instruction.
 *
 * @return InstructionInfo node.
 * @throw InvalidOperationException when the provided anyvalue has the wrong format.
 */
std::unique_ptr<InstructionInfo> ToInstructionInfoNode(
  const sup::dto::AnyValue& instr_info_anyvalue);

/**
 * @brief Convert the given InstructionInfo node to an AnyValue. This function ignores any
 * possible child instructions.
 *
 * @param instr_info InstructionInfo node.
 *
 * @return AnyValue representation of the InstructionInfo node.
 */
sup::dto::AnyValue ToAnyValueNode(const InstructionInfo& instr_info);

/**
 * @brief Create a unique member name for an Instruction to be used in a structured AnyValue.
 * An integer index is used to generate the member name to allow to easily deduce member order later.
 *
 * @param idx Index to use.
 *
 * @return Unique member name.
 */
std::string CreateIndexedInstrChildName(std::size_t idx);

/**
 * @brief Validate if the given AnyValue has the right format to be parsed as an InstructionInfo
 * node.
 *
 * @details This function only checks the top level node and does not validate any possible
 * children.
 *
 * @param instr_info AnyValue to validate.
 *
 * @return true when the provided AnyValue can be correctly parsed to a InstructionInfo node.
 */
bool ValidateInstructionInfoAnyValue(const sup::dto::AnyValue& instr_info);

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_INFO_UTILS_H_
