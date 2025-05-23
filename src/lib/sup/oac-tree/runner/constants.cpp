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

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/execution_status.h>

namespace sup
{
namespace oac_tree
{
namespace Constants
{
const dto::AnyValue kInstructionInfoNodeAnyValue = {{
  { kInstructionInfoNodeTypeField, "" },
  { kInstructionInfoNodeCategoryField, { sup::dto::UnsignedInteger32Type, 0 }},
  { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
  { kAttributesField, sup::dto::EmptyStruct() }
}, kInstructionInfoNodeType };

const dto::AnyValue kVariableInfoAnyValue = {{
  { kVariableInfoTypeField, "" },
  { kIndexField, { sup::dto::UnsignedInteger32Type, 0 }},
  { kAttributesField, sup::dto::EmptyStruct() }
}, kVariableInfoType };

const dto::AnyValue kJobInfoAnyValue = {{
  { kFullNameFieldName, "" },
  { kWorkspaceInfoFieldName, sup::dto::AnyValue{} },
  { kInstructionTreeInfoFieldName, sup::dto::AnyValue{} }
}, kJobInfoType };

const dto::AnyValue kInstructionStateAnyValue = {{
  { kExecStatusField, static_cast<dto::uint16>(ExecutionStatus::NOT_STARTED)},
  { kBreakpointField, false }
}, kInstructionStateType };

}  // namespace Constants

}  // namespace oac_tree

}  // namespace sup
