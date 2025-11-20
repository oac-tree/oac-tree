/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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

#include <sup/oac-tree/instruction_registry.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/sequence_parser.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::oac_tree;

class TestInstruction : public Instruction
{
public:
  TestInstruction() : Instruction(TestInstruction::Type) {}
  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface&, Workspace&) override
  {
    return ExecutionStatus::SUCCESS;
  }
};

template <>
DocomentationEntry sup::oac_tree::InstructionDocFor<TestInstruction>()
{
  DocomentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "TEST" }
  }};
  return doc;
}

const std::string TestInstruction::Type = "TestInstruction";

TEST(InstructionRegistry, RegisterInstruction)
{
  // no such name
  auto registered_names = GlobalInstructionRegistry().RegisteredInstructionNames();
  auto it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it == registered_names.end());

  // not possible to create instruction
  EXPECT_THROW(GlobalInstructionRegistry().Create(TestInstruction::Type),
               InvalidOperationException);

  // registration
  EXPECT_TRUE(RegisterGlobalInstruction<TestInstruction>());

  // second registration with the same name is not allowed
  EXPECT_THROW(RegisterGlobalInstruction<TestInstruction>(), InvalidOperationException);

  // there is a name
  registered_names = GlobalInstructionRegistry().RegisteredInstructionNames();
  it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_FALSE(it == registered_names.end());

  // can create instruction
  auto inst = GlobalInstructionRegistry().Create(TestInstruction::Type);
  EXPECT_TRUE(static_cast<bool>(inst));

  // Documentation object is as expected
  const auto doc = GlobalInstructionRegistry().GetDocumentation(TestInstruction::Type);
  const auto doc_fields = doc.GetFieldNames();
  ASSERT_EQ(doc_fields.size(), 1);
  EXPECT_EQ(doc_fields[0], Constants::INSTR_DOC_DESCRIPTION);
  EXPECT_EQ(doc.GetField(Constants::INSTR_DOC_DESCRIPTION), "TEST");
}

TEST(InstructionRegistry, CreateFailure)
{
  EXPECT_THROW(GlobalInstructionRegistry().Create("UndefinedInstructionName"),
               InvalidOperationException);
}
