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
* Copyright (c) : 2010-2020 ITER Organization,
*                 CS 90 046
*                 13067 St. Paul-lez-Durance Cedex
*                 France
*
* This file is part of ITER CODAC software.
* For the terms and conditions of redistribution or use of this software
* refer to the file ITER-LICENSE.TXT located in the top level directory
* of the distribution package.
******************************************************************************/


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <common/log-api.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ForceSuccess.h"


namespace sup {

namespace sequencer {

const std::string ForceSuccess::Type = "ForceSuccess";

ForceSuccess::ForceSuccess()
  : DecoratorInstruction(Type)
{}

ForceSuccess::~ForceSuccess() = default;

ExecutionStatus ForceSuccess::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
  _child->ExecuteSingle(ui, ws);
  auto status = _child->GetStatus();

  if (status == ExecutionStatus::FAILURE)
  {
    status = ExecutionStatus::SUCCESS;
  }
  return status;
}

}  // namespace sequencer

}  // namespace sup
