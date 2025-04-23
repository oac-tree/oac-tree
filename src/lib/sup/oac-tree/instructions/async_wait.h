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

#ifndef SUP_OAC_TREE_ASYNC_WAIT_H_
#define SUP_OAC_TREE_ASYNC_WAIT_H_

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction node that returns SUCCESS after a given timeout.
 *
 * @note As opposed to the Wait instruction, this instruction is implemented to behave
 * asynchronously, i.e. it will not block but immediately return RUNNING, unless the timeout was
 * exceeded or the instruction was halted.
 *
 * @details The 'timeout' attribute is optional. When this attribute is not present,
 * the instruction returns SUCCESS immediately.
 */
class AsyncWait : public Instruction
{
public:
  AsyncWait();

  ~AsyncWait() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  sup::dto::int64 m_finish;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ASYNC_WAIT_H_
