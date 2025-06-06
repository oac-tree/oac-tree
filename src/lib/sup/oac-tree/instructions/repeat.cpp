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

#include "repeat.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/generic_utils.h>

namespace sup
{
namespace oac_tree
{
const std::string Repeat::Type = "Repeat";

Repeat::Repeat()
  : DecoratorInstruction(Repeat::Type)
  , m_max_count{0}
  , m_count{0}
  , m_init_ok{false}
{
  AddAttributeDefinition(Constants::MAX_COUNT_ATTRIBUTE_NAME, sup::dto::SignedInteger32Type)
    .SetCategory(AttributeCategory::kBoth);
}

Repeat::~Repeat() = default;

bool Repeat::InitHook(UserInterface& ui, Workspace& ws)
{
  m_count = 0;
  m_max_count = 0;
  if (!GetAttributeValueAs(Constants::MAX_COUNT_ATTRIBUTE_NAME, ws, ui, m_max_count))
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
    ResetChild(ui);
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

}  // namespace oac_tree

}  // namespace sup
