/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#ifndef SUP_OAC_TREE_FAIL_H_
#define SUP_OAC_TREE_FAIL_H_

#include <sup/oac-tree/instruction.h>

#include <condition_variable>
#include <mutex>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction that returns failure.
 */
class Fail : public Instruction
{
public:
  Fail();

  ~Fail() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void HaltImpl() override;

  void ResetHook(UserInterface& ui) override;

  bool m_blocking;
  sup::dto::uint64 m_finish;
  std::mutex m_mtx;
  std::condition_variable m_cv;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_FAIL_H_
