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

#include "async.h"

namespace sup
{
namespace oac_tree
{
const std::string Async::Type = "Async";

Async::Async()
  : DecoratorInstruction(Async::Type)
  , m_wrapper{}
{}

Async::~Async() = default;

bool Async::InitHook(UserInterface& ui, Workspace& ws)
{
  (void)ui;
  (void)ws;
  m_wrapper = std::make_unique<AsyncWrapper>(GetChild());
  return true;
}

ExecutionStatus Async::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto wrapper_status = m_wrapper->GetStatus();
  if (NeedsExecute(wrapper_status))
  {
    m_wrapper->Tick(ui, ws);
  }
  return CalculateStatus();
}

void Async::ResetHook(UserInterface& ui)
{
  // call Halt when running asynchronously
  auto status = GetStatus();
  if (status == ExecutionStatus::RUNNING)
  {
    Halt();
  }
  // wait for child thread to terminate
  m_wrapper.reset();
  // call reset on child instruction
  ResetChild(ui);
}

ExecutionStatus Async::CalculateStatus() const
{
  auto wrapper_status = m_wrapper->GetStatus();
  if (IsFinishedStatus(wrapper_status))
  {
    return wrapper_status;
  }
  return ExecutionStatus::RUNNING;
}

}  // namespace oac_tree

}  // namespace sup
