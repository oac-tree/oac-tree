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

#ifndef SUP_OAC_TREE_WAIT_FOR_VARIABLE_H_
#define SUP_OAC_TREE_WAIT_FOR_VARIABLE_H_

#include <sup/oac-tree/instruction.h>

#include <sup/dto/basic_scalar_types.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction node that waits 'timeout' seconds for a variable to be readable and non-empty.
 *
 * @details The 'timeout' attribute is mandatory. There is also the 'equalsVar' option, that
 * will wait that the read variable is equal to the one provided in that variable.
 */
class WaitForVariable : public Instruction
{
public:
  WaitForVariable();

  ~WaitForVariable() override;

  static const std::string Type;

private:
  sup::dto::int64 m_finish;

  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook(UserInterface& ui) override;

  bool SuccessCondition(bool var_available, const sup::dto::AnyValue& var_value,
                        bool other_available, const sup::dto::AnyValue& other_value) const;

  bool CheckCondition(UserInterface& ui, Workspace& ws) const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_WAIT_FOR_VARIABLE_H_
