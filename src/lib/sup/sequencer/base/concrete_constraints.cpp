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

#include <sup/sequencer/concrete_constraints.h>

namespace sup
{
namespace sequencer
{

Exists::Exists(const std::string& attr_name)
  : m_attr_name{attr_name}
{}

Exists::~Exists() = default;

bool Exists::Validate(const ValueMap& attr_map) const
{
  return attr_map.find(m_attr_name) != attr_map.end();
}

std::string Exists::GetRepresentation() const
{
  return "Exists(" + m_attr_name + ")";
}

}  // namespace sequencer

}  // namespace sup
