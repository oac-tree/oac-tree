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

#include <sup/oac-tree/instruction_doc.h>

namespace sup
{
namespace oac_tree
{

InstructionDoc::InstructionDoc()
  : m_description{}
{}

InstructionDoc::~InstructionDoc() = default;

std::string InstructionDoc::GetDescription() const
{
  return m_description;
}

void InstructionDoc::SetDescription(const std::string& description)
{
  m_description = description;
}

}  // namespace oac_tree

}  // namespace sup
