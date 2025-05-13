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

#ifndef SUP_OAC_TREE_ASYNC_H_
#define SUP_OAC_TREE_ASYNC_H_

#include <sup/oac-tree/decorator_instruction.h>

#include <sup/oac-tree/instructions/async_wrapper.h>

#include <memory>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Decorator that makes the execution of its child asynchronous.
 */
class Async : public DecoratorInstruction
{
public:
  Async();

  ~Async() override;

  static const std::string Type;

private:
  std::unique_ptr<AsyncWrapper> m_wrapper;

  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook(UserInterface& ui) override;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ASYNC_H_
