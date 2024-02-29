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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/constraint.h>

namespace sup
{
namespace sequencer
{
IConstraint::~IConstraint() = default;

Constraint::~Constraint() = default;

Constraint::Constraint(const Constraint& other)
  : m_impl{other.m_impl->Clone()}
{}

Constraint::Constraint(Constraint&& other)
  : m_impl{std::move(other.m_impl)}
{}

Constraint& Constraint::operator=(const Constraint& other)
{
  Constraint copy{other};
  std::swap(m_impl, copy.m_impl);
  return *this;
}

Constraint& Constraint::operator=(Constraint&& other)
{
  std::swap(m_impl, other.m_impl);
  return *this;
}

bool Constraint::Validate(const StringAttributeList& attr_map) const
{
  if (!m_impl)
  {
    return false;
  }
  return m_impl->Validate(attr_map);
}

std::string Constraint::GetRepresentation() const
{
  if (!m_impl)
  {
    return kConstraintEmpty;
  }
  return m_impl->GetRepresentation();
}

Constraint::Constraint(std::unique_ptr<IConstraint>&& impl)
  : m_impl{std::move(impl)}
{}

}  // namespace sequencer

}  // namespace sup
