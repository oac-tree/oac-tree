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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "procedure_to_treedata_utils.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <sup/xml/tree_data.h>

#include <set>

namespace internal
{
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

std::unique_ptr<sup::xml::TreeData> CreateTreeData(const sup::sequencer::Instruction* instruction)
{
  auto tree_data = internal::make_unique<sup::xml::TreeData>(instruction->GetType());
  for (const auto& it : instruction->GetStringAttributes())
  {
    tree_data->AddAttribute(it.first, it.second);
  }
  return tree_data;
}

void Iterate(const sup::sequencer::Instruction* instruction, sup::xml::TreeData* parent)
{
  for (auto& child : instruction->ChildInstructions())
  {
    auto tree_data = CreateTreeData(child);
    Iterate(child, tree_data.get());
    parent->AddChild(*tree_data);
  }
}

}  // namespace internal

namespace sup
{
namespace sequencer
{
std::unique_ptr<sup::xml::TreeData> ToTreeData(const Procedure& procedure)
{
  auto result = internal::make_unique<sup::xml::TreeData>(Constants::PROCEDURE_ELEMENT_NAME);
  for (auto instruction : procedure.GetTopInstructions())
  {
    result->AddChild(*ToTreeData(*instruction));
  }
  result->AddChild(*ToTreeData(*procedure.GetWorkspace()));
  return result;
}

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Variable& variable)
{
  auto result = internal::make_unique<sup::xml::TreeData>(variable.GetType());
  for (const auto& it : variable.GetStringAttributes())
  {
    result->AddAttribute(it.first, it.second);
  }
  return result;
}

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Workspace& workspace)
{
  auto result = internal::make_unique<sup::xml::TreeData>(Constants::WORKSPACE_ELEMENT_NAME);
  for (auto variable : workspace.GetVariables())
  {
    result->AddChild(*ToTreeData(*variable));
  }
  return result;
}

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Instruction& instruction)
{
  auto tree_data = internal::CreateTreeData(&instruction);
  internal::Iterate(&instruction, tree_data.get());
  return tree_data;
}

}  // namespace sequencer

}  // namespace sup
