/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : Instruction node implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#include "copy.h"

const std::string INPUT_VARIABLE_ATTR_NAME = "inputVar";
const std::string OUTPUT_VARIABLE_ATTR_NAME = "outputVar";

namespace sup
{
namespace sequencer
{
const std::string Copy::Type = "Copy";

Copy::Copy()
  : Instruction(Copy::Type)
{
  AddAttributeDefinition(INPUT_VARIABLE_ATTR_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(OUTPUT_VARIABLE_ATTR_NAME, sup::dto::StringType).SetMandatory();
}

Copy::~Copy() = default;

ExecutionStatus Copy::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetValueFromAttributeName(*this, ws, ui, INPUT_VARIABLE_ATTR_NAME, value))
  {
    return ExecutionStatus::FAILURE;
  }
  if (!SetValueFromAttributeName(*this, ws, ui, OUTPUT_VARIABLE_ATTR_NAME, value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
