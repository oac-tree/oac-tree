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

namespace
{
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
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
  auto result = make_unique<TreeData>("Workspace");  // FIXME replace with constants
  for (auto variable : workspace.GetVariables())
  {
    result->AddChild(*ToTreeData(*variable));
  }

  return result;
}

}  // namespace sequencer

}  // namespace sup
