/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
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

#include <sup/sequencer/variable_utils.h>

#include <sup/sequencer/anyvalue_utils.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/workspace.h>

#include <algorithm>

namespace sup
{
namespace sequencer
{
using namespace Constants;

namespace utils
{
WorkspaceInfo CreateWorkspaceInfo(const Workspace& ws)
{
  WorkspaceInfo result;
  auto var_names = ws.VariableNames();
  sup::dto::uint32 idx = 0;
  for (const auto& var_name : var_names)
  {
    result.AddVariableInfo(var_name, CreateVariableInfo(ws.GetVariable(var_name), idx));
    ++idx;
  }
  return result;
}

WorkspaceInfo ToWorkspaceInfo(const sup::dto::AnyValue& ws_info_anyvalue)
{
  if (!ValidateWorkspaceInfoAnyValue(ws_info_anyvalue))
  {
    const std::string error = "ToWorkspaceInfo(): wrong format of workspace info AnyValue";
    throw InvalidOperationException(error);
  }
  WorkspaceInfo result;
  auto var_names = ws_info_anyvalue.MemberNames();
  for (const auto& var_name : var_names)
  {
    const auto& var_av = ws_info_anyvalue[var_name];
    auto var_info = ToVariableInfo(var_av);
    result.AddVariableInfo(var_name, var_info);
  }
  return result;
}

sup::dto::AnyValue ToAnyValue(const WorkspaceInfo& ws_info)
{
  sup::dto::AnyValue result = sup::dto::EmptyStruct();
  for (const auto& [var_name, var_info] : ws_info.GetVariableInfos())
  {
    auto var_av = ToAnyValue(var_info);
    result.AddMember(var_name, var_av);
  }
  return result;
}

bool ValidateWorkspaceInfoAnyValue(const sup::dto::AnyValue& ws_info)
{
  if (!sup::dto::IsStructValue(ws_info))
  {
    return false;
  }
  auto mem_names = ws_info.MemberNames();
  for (const auto& mem_name : mem_names)
  {
    if (!ValidateVariableInfoAnyValue(ws_info[mem_name]))
    {
      return false;
    }
  }
  return true;
}

std::vector<std::string> BuildVariableNameMap(const WorkspaceInfo& ws_info)
{
  const auto& var_infos = ws_info.GetVariableInfos();
  std::vector<std::string> result;
  auto func = [](const auto& pair) {
    return pair.first;
  };
  (void)std::transform(var_infos.begin(), var_infos.end(), std::back_inserter(result), func);
  return result;
}

VariableInfo CreateVariableInfo(const Variable* var, sup::dto::uint32 index)
{
  if (var == nullptr)
  {
    const std::string error = "CreateVariableInfo(): called with a nullptr";
    throw InvalidOperationException(error);
  }
  auto var_type = var->GetType();
  std::vector<AttributeInfo> attributes = ToAttributeInfos(var->GetStringAttributes());
  return VariableInfo{ var_type, index, attributes };
}

VariableInfo ToVariableInfo(const sup::dto::AnyValue& var_info_anyvalue)
{
  if (!ValidateVariableInfoAnyValue(var_info_anyvalue))
  {
    const std::string error = "ToVariableInfo(): wrong format of variable info AnyValue";
    throw InvalidOperationException(error);
  }
  auto var_type = var_info_anyvalue[kVariableInfoTypeField].As<std::string>();
  auto var_idx = var_info_anyvalue[kIndexField].As<sup::dto::uint32>();
  auto& attr_av = var_info_anyvalue[kAttributesField];
  std::vector<AttributeInfo> attributes;
  for (const auto& attr_name : attr_av.MemberNames())
  {
    attributes.emplace_back(attr_name, attr_av[attr_name].As<std::string>());
  }
  return VariableInfo{ var_type, var_idx, attributes };
}

sup::dto::AnyValue ToAnyValue(const VariableInfo& var_info)
{
  auto result = kVariableInfoAnyValue;
  result[kVariableInfoTypeField] = var_info.GetType();
  result[kIndexField] = var_info.GetIndex();
  for (const auto& [attr_name, attr_value] : var_info.GetAttributes())
  {
    result[kAttributesField].AddMember(attr_name, attr_value);
  }
  return result;
}

bool ValidateVariableInfoAnyValue(const sup::dto::AnyValue& var_info)
{
  if (!ValidateMemberType(var_info, kVariableInfoTypeField, sup::dto::StringType))
  {
    return false;
  }
  auto type_str = var_info[kVariableInfoTypeField].As<std::string>();
  if (type_str.empty())
  {
    return false;
  }
  if (!ValidateMemberType(var_info, kIndexField, sup::dto::UnsignedInteger32Type))
  {
    return false;
  }
  if (!var_info.HasField(kAttributesField) || !sup::dto::IsStructValue(var_info[kAttributesField]))
  {
    return false;
  }
  const auto& var_attrs = var_info[kAttributesField];
  auto mem_names = var_attrs.MemberNames();
  for (const auto& mem_name : mem_names)
  {
    if (var_attrs[mem_name].GetType() != sup::dto::StringType)
    {
      return false;
    }
  }
  return true;
}

}  // namespace utils

}  // namespace sequencer

}  // namespace sup
