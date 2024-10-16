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

#ifndef SUP_SEQUENCER_SUP_INFO_CONSTANTS_H_
#define SUP_SEQUENCER_SUP_INFO_CONSTANTS_H_

#include <sup/dto/anyvalue.h>

#include <string>

namespace sup
{
namespace sequencer
{
// Instruction info node fields
const std::string kInstructionInfoNodeType = "sup::instructionNodeInfoType/v1.0";
const std::string kInstructionInfoNodeTypeField = "type";
const std::string kIndexField = "index";
const std::string kAttributesField = "attributes";
const std::string kChildInstructionsField = "children";
const std::string kChildMemberFieldPrefix = "child_";

// Basic instruction info node AnyValue
extern const sup::dto::AnyValue kInstructionInfoNodeAnyValue;

// Variable info node fields
const std::string kVariableInfoType = "sup::variableInfoType/v1.0";
const std::string kVariableInfoTypeField = "type";

// Basic variable info node AnyValue
extern const sup::dto::AnyValue kVariableInfoAnyValue;

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_SUP_INFO_CONSTANTS_H_
