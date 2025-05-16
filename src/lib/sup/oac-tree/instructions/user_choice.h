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

#ifndef SUP_OAC_TREE_USER_CHOICE_H_
#define SUP_OAC_TREE_USER_CHOICE_H_

#include <sup/oac-tree/compound_instruction.h>

#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Compound instruction representing a choice of instructions.
 *
 * @note The selection of the instruction to execute is done by delegating this to
 * the UserInterface.
 */
class UserChoice : public CompoundInstruction
{
public:
  UserChoice();

  ~UserChoice() override;

  static const std::string Type;

private:
  sup::dto::int32 m_choice;
  std::unique_ptr<IUserInputFuture> m_future;

  bool InitHook(UserInterface& ui, Workspace& ws) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void ResetHook(UserInterface& ui) override;

  void HaltImpl() override;

  std::vector<const Instruction*> NextInstructionsImpl() const override;

  /**
   * @brief Construct a list of choices from the available child instructions.
   */
  std::vector<std::string> GetChoices() const;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_USER_CHOICE_H_
