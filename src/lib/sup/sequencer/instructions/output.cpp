/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "output.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

const std::string FROM_ATTRIBUTE_NAME{"fromVar"};
const std::string DESCR_ATTRIBUTE_NAME{"description"};

namespace sup
{
namespace sequencer
{
const std::string Output::Type = "Output";

Output::Output()
  : Instruction(Output::Type)
{
  AddAttributeDefinition(FROM_ATTRIBUTE_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(DESCR_ATTRIBUTE_NAME, sup::dto::StringType);
}

Output::~Output() = default;

ExecutionStatus Output::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  sup::dto::AnyValue value;
  if (!GetValueFromAttributeName(*this, ws, ui, FROM_ATTRIBUTE_NAME, value))
  {
    return ExecutionStatus::FAILURE;
  }
  auto description =
    HasAttribute(DESCR_ATTRIBUTE_NAME) ? GetAttributeValue<std::string>(DESCR_ATTRIBUTE_NAME)
                                       : "";
  return ui.PutValue(value, description) ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

}  // namespace sequencer

}  // namespace sup
