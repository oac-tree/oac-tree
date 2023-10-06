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
  , m_max_count{0}
  , m_count{0}
  , m_init_ok{false}
{
  AddAttributeDefinition(MAXCOUNT_ATTR_NAME, sup::dto::SignedInteger32Type)
    .SetCategory(AttributeCategory::kBoth);
}

Repeat::~Repeat() = default;

bool Repeat::InitHook(UserInterface& ui, Workspace& ws)
{
  m_count = 0;
  m_max_count = 0;
  if (!GetAttributeValueAs(MAXCOUNT_ATTR_NAME, ws, ui, m_max_count))
  {
    return false;
  }
  if (m_max_count < 0)
  {
    m_max_count = -1;
  }
  return true;
}

ExecutionStatus Repeat::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  if (m_max_count == 0)
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
  // Don't increment count when m_max_count is not strictly positive.
  if (m_max_count > 0
      && (child_status == ExecutionStatus::SUCCESS || child_status == ExecutionStatus::FAILURE))
  {
    m_count++;
  }
  return CalculateStatus();
}

ExecutionStatus Repeat::CalculateStatus() const
{
  auto child_status = GetChildStatus();
  if (child_status == ExecutionStatus::SUCCESS)
  {
    if (m_count == m_max_count)
    {
      return ExecutionStatus::SUCCESS;
    }
    return ExecutionStatus::NOT_FINISHED;
  }
  return child_status;
}

}  // namespace sequencer

}  // namespace sup
