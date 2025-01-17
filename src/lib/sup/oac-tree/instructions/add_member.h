/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Carlos Miguel Silva (EXT)
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

#ifndef SUP_OAC_TREE_ADDMEMBER_H_
#define SUP_OAC_TREE_ADDMEMBER_H_

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction for adding a new member to a workspace variable
 *
 * @details An AnyValue stored in "inputVar" will be added as member to the field "outputVar" under
 * the member name "memberName". If the destination field is not a struct, already has a member with
 * that name or if its type is locked, the instruction fails.
 */
class AddMember : public Instruction
{
public:
  AddMember();

  ~AddMember() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_ADDMEMBER_H_
