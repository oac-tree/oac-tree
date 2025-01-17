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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_SCOPE_GUARD_H_
#define SUP_OAC_TREE_SCOPE_GUARD_H_

#include <functional>

namespace sup
{
namespace oac_tree
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

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_SCOPE_GUARD_H_
