/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "ProcedureToTreeDataUtils.h"

#include "AttributeMap.h"
#include "Instruction.h"
#include "Procedure.h"
#include "TreeData.h"
#include "Variable.h"
#include "Workspace.h"
#include "Constants.h"

#include <set>

namespace
{
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

::sup::sequencer::TreeData* ProcessInstruction(const ::sup::sequencer::Instruction* instruction,
                                               ::sup::sequencer::TreeData* parent)
{
  auto tree_data = make_unique<::sup::sequencer::TreeData>(instruction->GetType());
  for (const auto& it : instruction->GetAttributes())
  {
    tree_data->AddAttribute(it.first, it.second);
  }

  auto next_parent = tree_data.get();
  parent->AddChild(*tree_data);
  return next_parent;
}

void Iterate(const ::sup::sequencer::Instruction* instruction, ::sup::sequencer::TreeData* parent)
{
  for (auto& instruction : instruction->ChildInstructions())
  {
    auto next_parent_item = ProcessInstruction(instruction, parent);
    if (next_parent_item)
    {
      Iterate(instruction, next_parent_item);
    }
  }
}

}  // namespace

namespace sup
{
namespace sequencer
{
std::unique_ptr<TreeData> ToTreeData(const Procedure& procedure)
{
  return {};
}

std::unique_ptr<TreeData> ToTreeData(const Variable& variable)
{
  auto result = make_unique<TreeData>(variable.GetType());
  for (const auto& it : variable.GetAttributes())
  {
    result->AddAttribute(it.first, it.second);
  }
  return result;
}

std::unique_ptr<TreeData> ToTreeData(const Workspace& workspace)
{
  auto result = make_unique<TreeData>(Constants::WORKSPACE_ELEMENT_NAME);
  for (auto variable : workspace.GetVariables())
  {
    result->AddChild(*ToTreeData(*variable));
  }

  return result;
}

std::unique_ptr<TreeData> ToTreeData(const Instruction& instruction)
{
  auto tree_data = make_unique<::sup::sequencer::TreeData>(instruction.GetType());
  for (const auto& it : instruction.GetAttributes())
  {
    tree_data->AddAttribute(it.first, it.second);
  }
  Iterate(&instruction, tree_data.get());

  return tree_data;
}

}  // namespace sequencer

}  // namespace sup
