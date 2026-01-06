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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <sup/oac-tree/breakpoint.h>

namespace sup
{
namespace oac_tree
{

Breakpoint::Breakpoint(const Instruction* instruction)
  : m_instruction{instruction}
{}

Breakpoint::~Breakpoint() = default;

const Instruction* Breakpoint::GetInstruction() const
{
  return m_instruction;
}

}  // namespace oac_tree

}  // namespace sup
