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
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

namespace
{
const std::string from_attribute_name{"from"};
const std::string descr_attribute_name{"description"};
}  // namespace

namespace sup
{
namespace sequencer
{
const std::string Output::Type = "Output";

ExecutionStatus Output::ExecuteSingleImpl(UserInterface* ui, Workspace* ws)
{
  bool status = HasAttribute(from_attribute_name);

  ccs::types::AnyValue value;

  if (status)
  {  // Read from workspace
    ws->GetValue(GetAttribute(from_attribute_name), value);
    status = value.GetSize() > 0;
  }

  if (status)
  {  // Read from workspace
    status = ui->PutValue(value, GetAttribute(descr_attribute_name));
  }

  return status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE;
}

Output::Output() : Instruction(Type) {}

Output::~Output() = default;

}  // namespace sequencer

}  // namespace sup
