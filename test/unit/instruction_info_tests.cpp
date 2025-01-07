/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
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

#include "unit_test_helper.h"

#include <sup/sequencer/constants.h>
#include <sup/sequencer/instruction_info_utils.h>
#include <sup/sequencer/instruction_map.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

#include <set>

using namespace sup::sequencer;
using namespace sup::sequencer::Constants;

class InstructionInfoTest : public ::testing::Test
{
protected:
  InstructionInfoTest() = default;
  virtual ~InstructionInfoTest() = default;
};

TEST_F(InstructionInfoTest, InstructionInfoFromInstructionTree)
{
  // Construct procedure and extract Workspace
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_NO_THROW(proc->Setup());
  const auto* root = proc->RootInstruction();

  // Create InstructionInfo tree
  InstructionMap instr_map{root};
  auto instr_info = utils::CreateInstructionInfoTree(*root, instr_map);
  EXPECT_NO_THROW(ValidateInstructionInfoTree(*instr_info));

  // Check root of the InstructionInfo tree
  EXPECT_EQ(instr_info->GetType(), "Sequence");
  std::set<sup::dto::uint32> indices;
  indices.insert(instr_info->GetIndex());
  auto attributes = instr_info->GetAttributes();
  EXPECT_EQ(attributes.size(), 0);

  auto children = instr_info->Children();
  ASSERT_EQ(children.size(), 2);
  {
    // check first child
    auto child = children[0];
    EXPECT_EQ(child->GetType(), "Copy");
    indices.insert(child->GetIndex());
    auto child_attrs = child->GetAttributes();
    EXPECT_EQ(child_attrs.size(), 2);
    EXPECT_EQ(child->Children().size(), 0);
  }
  {
    // check second child
    auto child = children[1];
    EXPECT_EQ(child->GetType(), "Copy");
    indices.insert(child->GetIndex());
    auto child_attrs = child->GetAttributes();
    EXPECT_EQ(child_attrs.size(), 2);
    EXPECT_EQ(child->Children().size(), 0);
  }
  std::set<sup::dto::uint32> expected_indices{0, 1, 2};
  EXPECT_EQ(indices, expected_indices);

  // Test copy
  InstructionInfo copy{*instr_info};
  EXPECT_EQ(copy, *instr_info);
  EXPECT_FALSE(copy != *instr_info);

  // Test move
  InstructionInfo moved{std::move(copy)};
  EXPECT_EQ(moved, *instr_info);
  EXPECT_FALSE(moved != *instr_info);
}

TEST_F(InstructionInfoTest, Flatten)
{
  // Construct procedure and extract Workspace
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_NO_THROW(proc->Setup());
  const auto* root = proc->RootInstruction();

  // Create InstructionInfo tree and a flat list of the nodes
  InstructionMap instr_map{root};
  auto instr_info = utils::CreateInstructionInfoTree(*root, instr_map);
  auto flattened_info = Flatten(*instr_info);
  EXPECT_EQ(flattened_info.size(), 3);

  // Check indices
  std::set<sup::dto::uint32> indices;
  for (const auto* node : flattened_info)
  {
    indices.insert(node->GetIndex());
  }
  std::set<sup::dto::uint32> expected_indices{0, 1, 2};
  EXPECT_EQ(indices, expected_indices);
}

TEST_F(InstructionInfoTest, CreateOrderedInstructionInfo)
{
  {
    // InstructionInfo tree with proper indices
    InstructionInfo sequence("sequence", 0, {});
    auto child0 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child0", 1, {}));
    auto child0_ptr = child0.get();
    auto child1 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child1", 2, {}));
    auto child1_ptr = child1.get();

    EXPECT_EQ(sequence.AppendChild(std::move(child0)), child0_ptr);
    EXPECT_EQ(sequence.AppendChild(std::move(child1)), child1_ptr);
    EXPECT_NO_THROW(ValidateInstructionInfoTree(sequence));

    auto ordered_info = utils::CreateOrderedInstructionInfo(sequence);
    ASSERT_EQ(ordered_info.size(), 3);

    EXPECT_EQ(ordered_info[0], &sequence);
    EXPECT_EQ(ordered_info[1], child0_ptr);
    EXPECT_EQ(ordered_info[2], child1_ptr);
  }
  {
    // InstructionInfo tree with duplicate indices
    InstructionInfo sequence("sequence", 0, {});
    auto child0 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child0", 1, {}));
    auto child0_ptr = child0.get();
    auto child1 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child1", 0, {}));
    auto child1_ptr = child1.get();

    EXPECT_EQ(sequence.AppendChild(std::move(child0)), child0_ptr);
    EXPECT_EQ(sequence.AppendChild(std::move(child1)), child1_ptr);

    EXPECT_THROW(ValidateInstructionInfoTree(sequence), InvalidOperationException);
    EXPECT_THROW(utils::CreateOrderedInstructionInfo(sequence), InvalidOperationException);
  }
  {
    // InstructionInfo tree with out-of-bounds indices
    InstructionInfo sequence("sequence", 0, {});
    auto child0 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child0", 1, {}));
    auto child0_ptr = child0.get();
    auto child1 = std::unique_ptr<InstructionInfo>(new InstructionInfo("child1", 3, {}));
    auto child1_ptr = child1.get();

    EXPECT_EQ(sequence.AppendChild(std::move(child0)), child0_ptr);
    EXPECT_EQ(sequence.AppendChild(std::move(child1)), child1_ptr);

    EXPECT_THROW(ValidateInstructionInfoTree(sequence), InvalidOperationException);
    EXPECT_THROW(utils::CreateOrderedInstructionInfo(sequence), InvalidOperationException);
  }
}

TEST_F(InstructionInfoTest, InstructionInfoToFromAnyValue)
{
  // Construct procedure and extract Workspace
  const auto procedure_string = sup::UnitTestHelper::CreateProcedureString(kWorkspaceSequenceBody);
  auto proc = ParseProcedureString(procedure_string);
  ASSERT_NE(proc.get(), nullptr);
  EXPECT_NO_THROW(proc->Setup());
  const auto* root = proc->RootInstruction();
  InstructionMap instr_map{root};
  auto instr_info = utils::CreateInstructionInfoTree(*root, instr_map);

  {
    // Correct InstructionInfo tree from real Instruction tree
    auto instr_av = utils::ToAnyValueTree(*instr_info);
    auto instr_info_read_back = utils::ToInstructionInfoTree(instr_av);
    EXPECT_EQ(*instr_info_read_back, *instr_info);
  }
  {
    // Create duplicate indices in AnyValue representation
    auto instr_av = utils::ToAnyValueTree(*instr_info);
    auto& child1_av = instr_av[kChildInstructionsField][utils::CreateIndexedInstrChildName(1)];
    child1_av[kIndexField].ConvertFrom(0);
    EXPECT_THROW(utils::ToInstructionInfoTree(instr_av), InvalidOperationException);
  }
  {
    // Create out-of-bounds indices in AnyValue representation
    auto instr_av = utils::ToAnyValueTree(*instr_info);
    auto& child1_av = instr_av[kChildInstructionsField][utils::CreateIndexedInstrChildName(1)];
    child1_av[kIndexField].ConvertFrom(3);
    EXPECT_THROW(utils::ToInstructionInfoTree(instr_av), InvalidOperationException);
  }
}

TEST_F(InstructionInfoTest, CreateInstructionInfoNode)
{
  {
    // Correct AnyValue for InstructionInfo node
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, "Instr_type" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42 }},
      { kAttributesField, {
        { "attr_1", "attr_1_value" },
        { "attr_2", "attr_2_value" }
      }}
    }, kInstructionInfoNodeType };
    EXPECT_TRUE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_NO_THROW(utils::ToInstructionInfoNode(instr_info_av));
  }
  {
    // Missing instruction type field
    sup::dto::AnyValue instr_info_av = {{
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42 }},
      { kAttributesField, {
        { "attr_1", "attr_1_value" },
        { "attr_2", "attr_2_value" }
      }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
  {
    // Wrong type for instruction type field
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, 5 },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42 }},
      { kAttributesField, {
        { "attr_1", "attr_1_value" },
        { "attr_2", "attr_2_value" }
      }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
  {
    // Missing index field
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, "Instr_type" },
      { kAttributesField, {
        { "attr_1", "attr_1_value" },
        { "attr_2", "attr_2_value" }
      }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
  {
    // Wrong type for index field
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, "Instr_type" },
      { kIndexField, { sup::dto::SignedInteger64Type, 42 }},
      { kAttributesField, {
        { "attr_1", "attr_1_value" },
        { "attr_2", "attr_2_value" }
      }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
  {
    // Missing attributes field
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, "Instr_type" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42 }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
  {
    // Wrong type for attributes field
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, "Instr_type" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42 }},
      { kAttributesField, { sup::dto::Float64Type, 3.14 }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
  {
    // Wrong type for attributes members
    sup::dto::AnyValue instr_info_av = {{
      { kInstructionInfoNodeTypeField, "Instr_type" },
      { kIndexField, { sup::dto::UnsignedInteger32Type, 42 }},
      { kAttributesField, {
        { "attr_1", { sup::dto::BooleanType, true } },
        { "attr_2", "attr_2_value" }
      }}
    }, kInstructionInfoNodeType };
    EXPECT_FALSE(utils::ValidateInstructionInfoAnyValue(instr_info_av));
    EXPECT_THROW(utils::ToInstructionInfoNode(instr_info_av), InvalidOperationException);
  }
}
