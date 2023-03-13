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

#include <sup/sequencer/scope_guard.h>

namespace sup
{
namespace sequencer
{
ScopeGuard::ScopeGuard() = default;

ScopeGuard::ScopeGuard(std::function<void()> func)
  : m_func{std::move(func)}
{}

ScopeGuard::~ScopeGuard()
{
  if (m_func)
  {
    m_func();
  }
}

ScopeGuard::ScopeGuard(ScopeGuard&& other)
  : m_func{other.m_func}
{}

ScopeGuard& ScopeGuard::operator=(ScopeGuard&& other)
{
  ScopeGuard moved{std::move(other)};
  Swap(moved);
  return *this;
}

void ScopeGuard::Swap(ScopeGuard& other)
{
  std::swap(m_func, other.m_func);
}

bool ScopeGuard::IsValid() const
{
  return static_cast<bool>(m_func);
}

}  // namespace sequencer

}  // namespace sup
