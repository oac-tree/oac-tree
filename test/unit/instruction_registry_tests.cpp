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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <sup/oac-tree/instructions/all_instructions.h>

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
DocumentationEntry sup::oac_tree::InstructionDocFor<TestInstruction>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "TEST" }
  }};
  return doc;
}

const std::string TestInstruction::Type = "TestInstruction";

TEST(InstructionRegistryTest, RegisterInstruction)
{
  // Copy global registry
  auto registry = GlobalInstructionRegistry();
  // no such name
  auto registered_names = registry.RegisteredInstructionNames();
  auto it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it == registered_names.end());

  // not possible to create instruction
  EXPECT_THROW(registry.Create(TestInstruction::Type),
               InvalidOperationException);

  // registration
  EXPECT_TRUE(RegisterInstruction<TestInstruction>(registry));

  // second registration with the same name is not allowed
  EXPECT_THROW(RegisterInstruction<TestInstruction>(registry), InvalidOperationException);

  // there is a name
  registered_names = registry.RegisteredInstructionNames();
  it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_FALSE(it == registered_names.end());

  // can create instruction
  auto inst = registry.Create(TestInstruction::Type);
  EXPECT_TRUE(static_cast<bool>(inst));

  // Documentation object is as expected
  const auto doc = registry.GetDocumentation(TestInstruction::Type);
  const auto doc_fields = doc.GetFieldNames();
  ASSERT_EQ(doc_fields.size(), 1);
  EXPECT_EQ(doc_fields[0], Constants::INSTR_DOC_DESCRIPTION);
  EXPECT_EQ(doc.GetField(Constants::INSTR_DOC_DESCRIPTION), "TEST");
}

TEST(InstructionRegistryTest, BaseInstructions)
{
  const auto registered_names = GlobalInstructionRegistry().RegisteredInstructionNames();
  const std::vector<std::string> ignore_list{ "Counter" };
  for (const auto& instr_name : registered_names)
  {
    // continue for ignored instructions
    if (std::find(ignore_list.begin(), ignore_list.end(), instr_name) != ignore_list.end())
    {
      continue;
    }
    // can create instruction
    auto inst = GlobalInstructionRegistry().Create(instr_name);
    EXPECT_TRUE(static_cast<bool>(inst));

    // Documentation object is as expected
    const auto doc = GlobalInstructionRegistry().GetDocumentation(instr_name);
    const auto doc_fields = doc.GetFieldNames();
    ASSERT_EQ(doc_fields.size(), 1) << "Incorrect number of doc entries for instruction "
      << instr_name;
    EXPECT_EQ(doc_fields[0], Constants::INSTR_DOC_DESCRIPTION);
  }
}

TEST(InstructionRegistryTest, CreateFailure)
{
  EXPECT_THROW(GlobalInstructionRegistry().Create("UndefinedInstructionName"),
               InvalidOperationException);
}

TEST(InstructionRegistryTest, InstructionDocs)
{
  InstructionRegistry registry{};
  // GetDocumentation throws for unknown instruction
  EXPECT_THROW(registry.GetDocumentation(TestInstruction::Type), InvalidOperationException);
  // Register test instruction and check documentation
  EXPECT_TRUE(RegisterInstruction<TestInstruction>(registry));
  auto doc = registry.GetDocumentation(TestInstruction::Type);
  EXPECT_EQ(doc.GetField(Constants::INSTR_DOC_DESCRIPTION), "TEST");
  // New field name does not exist yet
  const std::string field_name{"NewFieldName"};
  const std::string empty{};
  EXPECT_EQ(doc.GetField(field_name), empty);

  // Set new field in documentation
  const std::string field_val{"NewFieldValue"};
  registry.SetDocumentationField(TestInstruction::Type, field_name, field_val);
  doc = registry.GetDocumentation(TestInstruction::Type);
  EXPECT_EQ(doc.GetField(Constants::INSTR_DOC_DESCRIPTION), "TEST");
  EXPECT_EQ(doc.GetField(field_name), field_val);
}
