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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_INSTRUCTION_STATE_H_
#define SUP_OAC_TREE_INSTRUCTION_STATE_H_

#include <sup/oac-tree/execution_status.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace oac_tree
{

/**
 * @brief InstructionState holds the dynamic state information about a single instruction.
 *
 */
struct InstructionState
{
  bool m_breakpoint_set;
  ExecutionStatus m_execution_status;
};

bool operator==(const InstructionState& left, const InstructionState& right);
bool operator!=(const InstructionState& left, const InstructionState& right);

/**
 * @brief Encode an InstructionState object as an AnyValue.
 *
 * @param state InstructionState object.
 * @return AnyValue encoding.
 */
sup::dto::AnyValue ToAnyValue(const InstructionState& state);

/**
 * @brief Parse an AnyValue to an InstructionState object.
 *
 * @param state_av AnyValue encoding of InstructionState object.
 * @return Parsed InstructionState object.
 * @throws InvalidOperationException when the AnyValue was not correctly encoded.
 */
InstructionState ToInstructionState(const sup::dto::AnyValue& state_av);

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_INSTRUCTION_STATE_H_
