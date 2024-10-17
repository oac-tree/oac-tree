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

#include "unit_test_helper.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_map.h>
#include <sup/sequencer/instruction.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;


class InstructionMapTest : public ::testing::Test
{
protected:
  InstructionMapTest();
  virtual ~InstructionMapTest();
};

/**
 * @brief Instruction that has two children that point to the same instruction.
 */
class DuplicateChildInstruction : public Instruction
{
public:
  DuplicateChildInstruction() : Instruction("DuplicateChildInstruction")
                              , m_child{} {}
  ~DuplicateChildInstruction() = default;
private:
  ExecutionStatus ExecuteSingleImpl(UserInterface&, Workspace&) override {
    return ExecutionStatus::SUCCESS;
  }
  std::vector<const Instruction*> ChildInstructionsImpl() const {
    std::vector<const Instruction*> result;
    result.push_back(std::addressof(m_child));
    result.push_back(std::addressof(m_child));
    return result;
  }
  sup::UnitTestHelper::TestTreeInstruction m_child;
};

TEST_F(InstructionMapTest, Construction)
{
  using namespace sup::UnitTestHelper;
  {
    // Empty root
    InstructionMap instr_map{nullptr};
    EXPECT_EQ(instr_map.GetNumberOfInstructions(), 0);
    const auto& index_map = instr_map.GetInstructionMapping();
    EXPECT_TRUE(index_map.empty());
    EXPECT_THROW(instr_map.FindInstructionIndex(nullptr), InvalidOperationException);
  }
  {
    // Root without children
    auto root = CreateTestTreeInstruction("root");
    InstructionMap instr_map{root.get()};
    EXPECT_EQ(instr_map.GetNumberOfInstructions(), 1u);
    const auto& index_map = instr_map.GetInstructionMapping();
    EXPECT_EQ(index_map.size(), 1u);
    auto idx = instr_map.FindInstructionIndex(root.get());
    EXPECT_EQ(idx, 0);
  }
  {
    // Root with nullptr child
    auto root = CreateTestTreeInstruction("root");
    root->InsertInstruction({}, 0);
    EXPECT_THROW(InstructionMap{root.get()}, InvalidOperationException);
  }
  {
    // Root with duplicate child pointers
    DuplicateChildInstruction root{};
    EXPECT_THROW(InstructionMap{std::addressof(root)}, InvalidOperationException);
  }
  {
    // Root with multiple levels of child instructions
    auto root = CreateTestTreeInstruction("root");
    auto child_1 = CreateTestTreeInstruction("child_1");
    child_1->InsertInstruction(CreateTestTreeInstruction("a"), 0);
    child_1->InsertInstruction(CreateTestTreeInstruction("b"), 1);
    root->InsertInstruction(std::move(child_1), 0);
    auto child_2 = CreateTestTreeInstruction("child_2");
    root->InsertInstruction(std::move(child_2), 1);

    InstructionMap instr_map{root.get()};
    EXPECT_EQ(instr_map.GetNumberOfInstructions(), 5u);
    const auto& index_map = instr_map.GetInstructionMapping();
    EXPECT_EQ(index_map.size(), 5u);

    // Test indices range from 0 to 4
    std::vector<sup::dto::uint32> indices;
    for (const auto& map_entry : index_map)
    {
      EXPECT_NE(map_entry.first, nullptr);
      indices.push_back(map_entry.second);
    }
    std::vector<sup::dto::uint32> expected{0, 1, 2, 3, 4};
    EXPECT_TRUE(std::is_permutation(indices.begin(), indices.end(), expected.begin()));
  }
}

InstructionMapTest::InstructionMapTest() = default;

InstructionMapTest::~InstructionMapTest() = default;
