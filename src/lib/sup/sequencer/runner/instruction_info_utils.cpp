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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/instruction_info_utils.h>

#include <sup/sequencer/anyvalue_utils.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/info_constants.h>
#include <sup/sequencer/instruction_map.h>

#include <sup/sequencer/instruction.h>

#include <algorithm>
#include <deque>

namespace
{
using namespace sup::sequencer;

struct InstrToInstrInfoStackNode
{
  const sup::sequencer::Instruction& instruction;
  InstructionInfo& instr_info;
};

struct AnyValueToInstrInfoStackNode
{
  const sup::dto::AnyValue& anyvalue;
  InstructionInfo& instr_info;
};

struct InstrInfoToAnyValueStackNode
{
  const InstructionInfo& instr_info;
  sup::dto::AnyValue& anyvalue;
};

}  // unnamed namespace

namespace sup
{
namespace sequencer
{
namespace utils
{
std::unique_ptr<InstructionInfo> CreateInstructionInfoTree(const sequencer::Instruction& root,
                                                           const InstructionMap& instr_map)
{
  // No explicit validation occurs here, since the InstructionMap ensure valid pointers and indices.
  std::deque<InstrToInstrInfoStackNode> stack;
  auto root_address = std::addressof(root);
  auto root_info =
    CreateInstructionInfoNode(root, instr_map.FindInstructionIndex(root_address));
  InstrToInstrInfoStackNode root_node{ root, *root_info };
  stack.push_back(root_node);
  while (!stack.empty())
  {
    auto node = stack.back();
    stack.pop_back();
    auto children = node.instruction.ChildInstructions();
    for (auto child : children)
    {
      auto child_info = CreateInstructionInfoNode(*child, instr_map.FindInstructionIndex(child));
      auto child_info_p = node.instr_info.AppendChild(std::move(child_info));
      InstrToInstrInfoStackNode child_node{*child, *child_info_p};
      stack.push_back(child_node);
    }
  }
  return root_info;
}

std::unique_ptr<InstructionInfo> ToInstructionInfoTree(
  const sup::dto::AnyValue& instr_info_anyvalue)
{
  std::deque<AnyValueToInstrInfoStackNode> stack;
  auto root_info = ToInstructionInfoNode(instr_info_anyvalue);
  AnyValueToInstrInfoStackNode root_node{ instr_info_anyvalue, *root_info };
  stack.push_back(root_node);
  while (!stack.empty())
  {
    auto node = stack.back();
    stack.pop_back();
    if (node.anyvalue.HasField(kChildInstructionsField))
    {
      auto& children_av = node.anyvalue[kChildInstructionsField];
      auto mem_names = children_av.MemberNames();
      for (const auto& mem_name : mem_names)
      {
        auto& child_av = children_av[mem_name];
        auto child_info = ToInstructionInfoNode(child_av);
        auto child_info_p = node.instr_info.AppendChild(std::move(child_info));
        AnyValueToInstrInfoStackNode child_node{ child_av, *child_info_p };
        stack.push_back(child_node);
      }
    }
  }
  ValidateInstructionInfoTree(*root_info);
  return root_info;
}

sup::dto::AnyValue ToAnyValueTree(const InstructionInfo& instr_info)
{
  std::deque<InstrInfoToAnyValueStackNode> stack;
  auto root_av = ToAnyValueNode(instr_info);
  InstrInfoToAnyValueStackNode root_node{ instr_info, root_av };
  stack.push_back(root_node);
  while (!stack.empty())
  {
    auto node = stack.back();
    stack.pop_back();
    auto children = node.instr_info.Children();
    if (children.empty())
    {
      continue;
    }
    node.anyvalue.AddMember(kChildInstructionsField, sup::dto::EmptyStruct());
    std::size_t child_idx = 0;
    auto& children_av = node.anyvalue[kChildInstructionsField];
    for (const auto* child : children)
    {
      auto child_av = ToAnyValueNode(*child);
      auto child_mem_name = CreateIndexedInstrChildName(child_idx);
      children_av.AddMember(child_mem_name, child_av);
      auto& child_av_ref = children_av[child_mem_name];
      InstrInfoToAnyValueStackNode child_node{ *child, child_av_ref };
      stack.push_back(child_node);
      ++child_idx;
    }
  }
  return root_av;
}

std::vector<const InstructionInfo*> CreateOrderedInstructionInfo(const InstructionInfo& instr_info)
{
  auto flat_list = Flatten(instr_info);
  std::vector<const InstructionInfo*> result(flat_list.size(), nullptr);
  for (const auto* info : flat_list)
  {
    auto idx = info->GetIndex();
    if (idx >= result.size())
    {
      const std::string error = "CreateOrderedInstructionInfo(): encountered index out of bounds";
      throw InvalidOperationException(error);
    }
    result[idx] = info;
  }
  auto iter = std::find(result.begin(), result.end(), nullptr);
  if (iter != result.end())
  {
    const std::string error = "CreateOrderedInstructionInfo(): encountered duplicate index";
    throw InvalidOperationException(error);
  }
  return result;
}

std::unique_ptr<InstructionInfo> CreateInstructionInfoNode(const sequencer::Instruction& instr,
                                                           sup::dto::uint32 index)
{
  auto instr_type = instr.GetType();
  std::vector<AttributeInfo> attributes = ToAttributeInfos(instr.GetStringAttributes());
  return std::unique_ptr<InstructionInfo>{new InstructionInfo{instr_type, index, attributes}};
}

std::unique_ptr<InstructionInfo> ToInstructionInfoNode(
  const sup::dto::AnyValue& instr_info_anyvalue)
{
  if (!ValidateInstructionInfoAnyValue(instr_info_anyvalue))
  {
    const std::string error = "ToInstructionInfoNode(): wrong format of instruction info AnyValue";
    throw InvalidOperationException(error);
  }
  auto instr_type = instr_info_anyvalue[kInstructionInfoNodeTypeField].As<std::string>();
  auto instr_idx = instr_info_anyvalue[kIndexField].As<sup::dto::uint32>();
  auto& attr_av = instr_info_anyvalue[kAttributesField];
  std::vector<AttributeInfo> attributes;
  for (const auto& attr_name : attr_av.MemberNames())
  {
    attributes.emplace_back(attr_name, attr_av[attr_name].As<std::string>());
  }
  return std::unique_ptr<InstructionInfo>{new InstructionInfo{instr_type, instr_idx, attributes}};
}

sup::dto::AnyValue ToAnyValueNode(const InstructionInfo& instr_info)
{
  auto result = kInstructionInfoNodeAnyValue;
  result[kInstructionInfoNodeTypeField] = instr_info.GetType();
  result[kIndexField] = instr_info.GetIndex();
  for (const auto& attr : instr_info.GetAttributes())
  {
    result[kAttributesField].AddMember(attr.first, attr.second);
  }
  return result;
}

std::string CreateIndexedInstrChildName(std::size_t idx)
{
  return kChildMemberFieldPrefix + std::to_string(idx);
}

bool ValidateInstructionInfoAnyValue(const sup::dto::AnyValue& instr_info)
{
  if (!ValidateMemberType(instr_info, kInstructionInfoNodeTypeField, sup::dto::StringType))
  {
    return false;
  }
  if (!ValidateMemberType(instr_info, kIndexField, sup::dto::UnsignedInteger32Type))
  {
    return false;
  }
  if (!instr_info.HasField(kAttributesField) ||
      !sup::dto::IsStructValue(instr_info[kAttributesField]))
  {
    return false;
  }
  const auto& instr_attrs = instr_info[kAttributesField];
  auto mem_names = instr_attrs.MemberNames();
  for (const auto& mem_name : mem_names)
  {
    if (instr_attrs[mem_name].GetType() != sup::dto::StringType)
    {
      return false;
    }
  }
  if (instr_info.HasField(kChildInstructionsField))
  {
    if (!sup::dto::IsStructValue(instr_info[kChildInstructionsField]))
    {
      return false;
    }
    auto child_mem_names = instr_info[kChildInstructionsField].MemberNames();
    for (std::size_t idx = 0; idx < child_mem_names.size(); ++idx)
    {
      if (child_mem_names[idx] != CreateIndexedInstrChildName(idx))
      {
        return false;
      }
    }
  }
  return true;
}

}  // namespace utils

}  // namespace sequencer

}  // namespace sup
