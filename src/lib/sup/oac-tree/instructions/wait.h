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

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction node that returns SUCCESS after a given timeout.
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
  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  int m_timing_accuracy;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_WAIT_H_
