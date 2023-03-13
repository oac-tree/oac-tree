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

#ifndef SUP_SEQUENCER_SCOPE_GUARD_H_
#define SUP_SEQUENCER_SCOPE_GUARD_H_

#include <functional>

namespace sup
{
namespace sequencer
{

/**
 * @brief Guard that calls a function on destruction.
 */
class ScopeGuard
{
public:
  ScopeGuard();
  explicit ScopeGuard(std::function<void()> func);
  ~ScopeGuard();

  ScopeGuard(ScopeGuard&& other);
  ScopeGuard& operator=(ScopeGuard&& other);

  ScopeGuard(const ScopeGuard& other) = delete;
  ScopeGuard& operator=(const ScopeGuard& other) = delete;

  void Swap(ScopeGuard& other);

  bool IsValid() const;

private:
  std::function<void()> m_func;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_SCOPE_GUARD_H_