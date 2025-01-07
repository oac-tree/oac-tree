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

#include <sup/sequencer/instruction_state.h>

#include <sup/sequencer/anyvalue_utils.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>

namespace sup
{
namespace sequencer
{
using namespace Constants;

bool operator==(const InstructionState& left, const InstructionState& right)
{
  return left.m_breakpoint_set == right.m_breakpoint_set &&
         left.m_execution_status == right.m_execution_status;
}

bool operator!=(const InstructionState& left, const InstructionState& right)
{
  return !(left == right);
}

sup::dto::AnyValue ToAnyValue(const InstructionState& state)
{
  auto result = kInstructionStateAnyValue;
  result[kExecStatusField] = static_cast<sup::dto::uint16>(state.m_execution_status);
  result[kBreakpointField] = state.m_breakpoint_set;
  return result;
}

InstructionState ToInstructionState(const sup::dto::AnyValue& state_av)
{
  if (!utils::ValidateMemberType(state_av, kExecStatusField, sup::dto::UnsignedInteger16Type))
  {
    const std::string error = "ToInstructionState(): could not parse provided AnyValue to an "
      "InstructionState object";
    throw InvalidOperationException(error);
  }
  if (!utils::ValidateMemberType(state_av, kBreakpointField, sup::dto::BooleanType))
  {
    const std::string error = "ToInstructionState(): could not parse provided AnyValue to an "
      "InstructionState object";
    throw InvalidOperationException(error);
  }
  InstructionState result;
  result.m_breakpoint_set = state_av[kBreakpointField].As<sup::dto::boolean>();
  result.m_execution_status =
    static_cast<sequencer::ExecutionStatus>(state_av[kExecStatusField].As<sup::dto::uint16>());
  return result;
}

}  // namespace sequencer

}  // namespace sup
