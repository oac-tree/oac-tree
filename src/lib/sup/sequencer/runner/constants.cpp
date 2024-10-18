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

#include <sup/sequencer/constants.h>
#include <sup/sequencer/execution_status.h>

namespace sup
{
namespace sequencer
{
namespace Constants
{
const dto::AnyValue kInstructionInfoNodeAnyValue = {{
  { kInstructionInfoNodeTypeField, "" },
  { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
  { kAttributesField, sup::dto::EmptyStruct() }
}, kInstructionInfoNodeType };

const dto::AnyValue kVariableInfoAnyValue = {{
  { kVariableInfoTypeField, "" },
  { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
  { kAttributesField, sup::dto::EmptyStruct() }
}, kVariableInfoType };

const dto::AnyValue kJobInfoAnyValue = {{
  { kJobPrefixFieldName, "" },
  { kFullNameFieldName, "" },
  { kWorkspaceInfoFieldName, sup::dto::AnyValue{} },
  { kInstructionTreeInfoFieldName, sup::dto::AnyValue{} }
}, kJobInfoType };

const dto::AnyValue kInstructionStateAnyValue = {{
  { kExecStatusField, static_cast<dto::uint16>(ExecutionStatus::NOT_STARTED)},
  { kBreakpointField, false }
}, kInstructionStateType };

}  // namespace Constants

}  // namespace sequencer

}  // namespace sup
