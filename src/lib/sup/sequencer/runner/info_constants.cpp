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

#include <sup/sequencer/info_constants.h>

namespace sup
{
namespace sequencer
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

}  // namespace sequencer

}  // namespace sup
