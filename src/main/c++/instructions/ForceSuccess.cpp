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

// Global header files

#include "log.h"

// Local header files

#include "ForceSuccess.h"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

const std::string ForceSuccess::Type = "ForceSuccess";

ForceSuccess::ForceSuccess() : DecoratorInstruction(Type) {}

ForceSuccess::~ForceSuccess() = default;

ExecutionStatus ForceSuccess::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
  if (!HasChild())
  {
    return ExecutionStatus::SUCCESS;
  }
  ExecuteChild(ui, ws);
  auto status = GetChildStatus();

  if (status == ExecutionStatus::FAILURE)
  {
    status = ExecutionStatus::SUCCESS;
  }
  return status;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
