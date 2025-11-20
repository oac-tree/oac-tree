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

#ifndef SUP_OAC_TREE_WAIT_H_
#define SUP_OAC_TREE_WAIT_H_

#include <sup/oac-tree/instruction.h>

#include <condition_variable>
#include <mutex>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction that returns SUCCESS after a given timeout.
 *
 * @details The 'timeout' attribute is optional. When this attribute is not present,
 * the instruction returns SUCCESS immediately.
 */
class Wait : public Instruction
{
public:
  Wait();

  ~Wait() override;

  static const std::string Type;

private:
  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void HaltImpl() override;

  void ResetHook(UserInterface& ui) override;

  bool m_blocking;
  sup::dto::int64 m_finish;
  std::mutex m_mtx;
  std::condition_variable m_cv;
};

template <>
inline DocumentationEntry InstructionDocFor<Wait>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction that returns SUCCESS after a given timeout" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_WAIT_H_
