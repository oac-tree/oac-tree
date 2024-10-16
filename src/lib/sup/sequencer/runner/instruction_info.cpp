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

#include <sup/sequencer/instruction_info.h>
#include <sup/sequencer/exceptions.h>

#include <algorithm>
#include <deque>
#include <set>

namespace
{
using namespace sup::sequencer;

struct InstrInfoCopyNode
{
  const InstructionInfo& src;
  InstructionInfo& dest;
};

}  // unnamed namespace

namespace sup
{
namespace sequencer
{

InstructionInfo::InstructionInfo(const std::string& instr_type, sup::dto::uint32 idx,
                                 std::vector<AttributeInfo> attributes)
  : m_instr_type{instr_type}
  , m_index{idx}
  , m_attributes{std::move(attributes)}
  , m_children{}
{}

InstructionInfo::~InstructionInfo() = default;

InstructionInfo::InstructionInfo(const InstructionInfo& other)
  : m_instr_type{other.m_instr_type}
  , m_index{other.m_index}
  , m_attributes{other.m_attributes}
  , m_children{}
{
  std::deque<InstrInfoCopyNode> stack;
  InstrInfoCopyNode root_node{ other, *this };
  stack.push_back(root_node);
  while (!stack.empty())
  {
    auto node = stack.back();
    stack.pop_back();
    auto children = node.src.Children();
    for (auto child : children)
    {
      std::unique_ptr<InstructionInfo> child_info{
        new InstructionInfo{child->GetType(), child->GetIndex(), child->GetAttributes()}};
      auto child_info_p = node.dest.AppendChild(std::move(child_info));
      InstrInfoCopyNode child_node{*child, *child_info_p};
      stack.push_back(child_node);
    }
  }
}

InstructionInfo::InstructionInfo(InstructionInfo&&) = default;

InstructionInfo& InstructionInfo::operator=(const InstructionInfo& other)
{
  InstructionInfo copy{other};
  return this->operator=(std::move(copy));
}

InstructionInfo& InstructionInfo::operator=(InstructionInfo&& other) = default;

std::string InstructionInfo::GetType() const
{
  return m_instr_type;
}

sup::dto::uint32 InstructionInfo::GetIndex() const
{
  return m_index;
}

std::vector<AttributeInfo> InstructionInfo::GetAttributes() const
{
  return m_attributes;
}

InstructionInfo* InstructionInfo::AppendChild(std::unique_ptr<InstructionInfo> child)
{
  m_children.push_back(std::move(child));
  return m_children.back().get();
}

std::vector<InstructionInfo*> InstructionInfo::Children()
{
  std::vector<InstructionInfo*> result;
  auto func = [](const std::unique_ptr<InstructionInfo>& instr) {
    return instr.get();
  };
  std::transform(m_children.begin(), m_children.end(), std::back_inserter(result), func);
  return result;
}

std::vector<const InstructionInfo*> InstructionInfo::Children() const
{
  std::vector<const InstructionInfo*> result;
  auto func = [](const std::unique_ptr<InstructionInfo>& instr) {
    return instr.get();
  };
  std::transform(m_children.begin(), m_children.end(), std::back_inserter(result), func);
  return result;
}

std::vector<const InstructionInfo*> Flatten(const InstructionInfo& instr_info_tree)
{
  std::vector<const InstructionInfo*> result;
  std::deque<const InstructionInfo*> stack;
  std::set<const InstructionInfo*> info_pointers;
  auto root = std::addressof(instr_info_tree);
  stack.push_back(root);
  while (!stack.empty())
  {
    auto p_info = stack.back();
    stack.pop_back();
    if (p_info == nullptr)
    {
      const std::string error = "Flatten(const InstructionInfo&): nullptr node found";
      throw InvalidOperationException(error);
    }
    result.push_back(p_info);
    info_pointers.insert(p_info);

    for (const auto child : p_info->Children())
    {
      stack.push_back(child);
    }
  }
  if (info_pointers.size() != result.size())
  {
      const std::string error = "Flatten(const InstructionInfo&): duplicate pointers found";
      throw InvalidOperationException(error);
  }
  return result;
}

void ValidateInstructionInfoTree(const InstructionInfo& instr_info_tree)
{
  // Flatten already checks for nullptr or duplicate InstructionInfo node pointers:
  auto info_list = Flatten(instr_info_tree);
  auto n_nodes = info_list.size();
  std::set<sup::dto::uint32> indices;
  for (const auto* info_pointer : info_list)
  {
    if (info_pointer->GetIndex() >= n_nodes)
    {
      const std::string error = "ValidateInstructionInfoTree(): index out of range found";
      throw InvalidOperationException(error);
    }
    indices.insert(info_pointer->GetIndex());
  }
  if (indices.size() != n_nodes)
  {
    const std::string error = "ValidateInstructionInfoTree(): duplicate indices found";
    throw InvalidOperationException(error);
  }
}

bool operator==(const InstructionInfo& left, const InstructionInfo& right)
{
  if ( (left.GetType() != right.GetType())
    || (left.GetIndex() != right.GetIndex())
    || (left.GetAttributes() != right.GetAttributes()))
  {
    return false;
  }
  auto left_children = left.Children();
  auto right_children = right.Children();
  if (left_children.size() != right_children.size())
  {
    return false;
  }
  // This recursive implementation is not very performant. However, it is currently only used
  // in unit testing.
  for (std::size_t i=0; i<left_children.size(); ++i)
  {
    if (*left_children[i] != *right_children[i])
    {
      return false;
    }
  }
  return true;
}

bool operator!=(const InstructionInfo& left, const InstructionInfo& right)
{
  return !(left == right);
}

}  // namespace sequencer

}  // namespace sup
