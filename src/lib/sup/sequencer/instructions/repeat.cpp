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

#include "repeat.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/generic_utils.h>

const std::string MAXCOUNT_ATTR_NAME = "maxCount";

namespace sup
{
namespace sequencer
{
const std::string Repeat::Type = "Repeat";

Repeat::Repeat()
  : DecoratorInstruction(Repeat::Type)
  , _max_count{0}
  , _count{0}
  , _init_ok{false}
{}

Repeat::~Repeat() = default;

void Repeat::InitHook()
{
  _count = 0;
}

void Repeat::SetupImpl(const Procedure& proc)
{
  if (HasAttribute(MAXCOUNT_ATTR_NAME))
  {
    _max_count = InstructionAttributeToInt(*this, MAXCOUNT_ATTR_NAME);
    if (_max_count < 0)
    {
      _max_count = -1;
    }
  }
  SetupChild(proc);
}

ExecutionStatus Repeat::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (!HasChild() || _max_count == 0)
  {
    return ExecutionStatus::SUCCESS;
  }
  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    ResetChild();
  }
  ExecuteChild(ui, ws);

  child_status = GetChildStatus();
  // Don't increment count when _max_count is not strictly positive.
  if (_max_count > 0
      && (child_status == ExecutionStatus::SUCCESS || child_status == ExecutionStatus::FAILURE))
  {
    _count++;
  }
  return CalculateStatus();
}

ExecutionStatus Repeat::CalculateStatus() const
{
  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    if (_count == _max_count)
    {
      return ExecutionStatus::SUCCESS;
    }
    return ExecutionStatus::NOT_FINISHED;
  }
  return child_status;
}

}  // namespace sequencer

}  // namespace sup
