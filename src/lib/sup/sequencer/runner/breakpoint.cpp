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

#include "breakpoint.h"

namespace sup
{
namespace sequencer
{

Breakpoint::Breakpoint(const Instruction* instruction)
  : m_instruction{instruction}
  , m_status{kSet}
{}

Breakpoint::~Breakpoint() = default;

const Instruction* Breakpoint::GetInstruction() const
{
  return m_instruction;
}

Breakpoint::Status Breakpoint::GetStatus() const
{
  return m_status;
}

void Breakpoint::SetStatus(const Status& status)
{
  m_status = status;
}

}  // namespace sequencer

}  // namespace sup
