/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
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

#include <sup/oac-tree/procedure.h>

#include "mock_user_interface.h"
#include "unit_test_helper.h"

#include <sup/oac-tree/variables/local_variable.h>
#include <sup/oac-tree/instructions/wait.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/procedure_context.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable.h>
#include <sup/oac-tree/variable_registry.h>

#include <sup/xml/exceptions.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <thread>

using namespace sup::oac_tree;

class ProcedureTest : public ::testing::Test
{
protected:
  ProcedureTest();
  virtual ~ProcedureTest();

  MockUserInterface mock_ui;
  Procedure empty_proc;
  std::unique_ptr<Procedure> loaded_proc;
  std::unique_ptr<Instruction> wait;
  std::unique_ptr<Variable> var1;
  std::unique_ptr<Variable> var2;
};

const std::string ProcedureString{R"(
    <Wait name="Not Used" timeout="1.0"/>
    <Sequence name="Main Sequence" isRoot="Yes">
        <Wait name="Immediate Success"/>
        <Wait name="Wait 10ms" timeout="0.01"/>
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"vartype1","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"vartype2","attributes":[{"value":{"type":"string"}}]}' />
        <Local name="var3" />
    </Workspace>
)"};

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;

TEST_F(ProcedureTest, DefaultConstructed)
{
  // Test constructed
  EXPECT_TRUE(empty_proc.GetFilename().empty());
  EXPECT_EQ(empty_proc.GetStatus(), ExecutionStatus::SUCCESS);

  // Empty instruction list
  EXPECT_EQ(empty_proc.RootInstruction(), nullptr);
  auto instructions = empty_proc.GetTopInstructions();
  EXPECT_EQ(instructions.size(), 0);
  EXPECT_EQ(empty_proc.GetInstructionCount(), 0);

  // Add one instruction
  EXPECT_THROW(empty_proc.PushInstruction(nullptr), InvalidOperationException);
  Instruction *p_wait = wait.get();
  EXPECT_NO_THROW(empty_proc.PushInstruction(std::move(wait)));
  EXPECT_EQ(empty_proc.RootInstruction(), p_wait);
  instructions = empty_proc.GetTopInstructions();
  EXPECT_EQ(instructions.size(), 1);
  std::string wrong_filename = "WrongFile.xml";
  auto proc_context = empty_proc.GetContext();
  EXPECT_THROW(proc_context.GetProcedure(wrong_filename), sup::xml::ParseException);

  // Variables
  auto variables = empty_proc.VariableNames();
  EXPECT_EQ(variables.size(), 0);
  std::string var1_name = "var1";
  std::string var2_name = "var2";
  EXPECT_TRUE(empty_proc.AddVariable(var1_name, std::move(var1)));
  variables = empty_proc.VariableNames();
  EXPECT_EQ(variables.size(), 1);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_TRUE(empty_proc.AddVariable(var2_name, std::move(var2)));
  variables = empty_proc.VariableNames();
  EXPECT_EQ(variables.size(), 2);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
}

TEST_F(ProcedureTest, SetupFailure)
{
  {
    // Attribute validation fails
    Procedure procedure;
    EXPECT_TRUE(procedure.AddAttribute(kTickTimeoutAttributeName, "cannot_parse_this"));
    EXPECT_THROW(procedure.Setup(), ProcedureSetupException);
  }
  {
    // Absence of root instruction causes failure
    Procedure procedure;
    EXPECT_THROW(procedure.Setup(), ProcedureSetupException);
  }
}

TEST_F(ProcedureTest, GetWorkspace)
{
  Procedure procedure;
  auto variable = GlobalVariableRegistry().Create("Local");
  std::vector<const Variable*> expected({variable.get()});
  procedure.AddVariable("var", std::move(variable));

  EXPECT_EQ(procedure.GetWorkspace().GetVariables(), expected);
}

TEST_F(ProcedureTest, InsertInstruction)
{
  Procedure procedure;

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(procedure.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0));
  EXPECT_EQ(procedure.GetInstructionCount(), 1);

  auto child1 = new Wait;
  EXPECT_TRUE(procedure.InsertInstruction(std::unique_ptr<Instruction>{child1}, 1));
  EXPECT_EQ(procedure.GetInstructionCount(), 2);

  // inserting child in between
  auto child2 = new Wait;
  EXPECT_TRUE(procedure.InsertInstruction(std::unique_ptr<Instruction>{child2}, 1));
  EXPECT_EQ(procedure.GetInstructionCount(), 3);

  EXPECT_EQ(procedure.GetTopInstructions(),
            std::vector<const Instruction *>({child0, child2, child1}));

  // wrong insert index
  EXPECT_FALSE(procedure.InsertInstruction(nullptr, -1));
  EXPECT_FALSE(procedure.InsertInstruction(nullptr, 4));
}

TEST_F(ProcedureTest, TakeMiddleChild)
{
  Procedure procedure;

  // inserting 4 children
  auto child0 = new Wait;
  procedure.InsertInstruction(std::unique_ptr<Instruction>{child0}, 0);
  auto child1 = new Wait;
  procedure.InsertInstruction(std::unique_ptr<Instruction>{child1}, 1);
  auto child2 = new Wait;
  procedure.InsertInstruction(std::unique_ptr<Instruction>{child2}, 2);
  auto child3 = new Wait;
  procedure.InsertInstruction(std::unique_ptr<Instruction>{child3}, 3);

  // taking middle child
  auto child1_taken = procedure.TakeInstruction(1);
  EXPECT_EQ(child1, child1_taken.get());
  EXPECT_EQ(procedure.GetTopInstructions().size(), 3);
  EXPECT_EQ(procedure.GetTopInstructions(),
            std::vector<const Instruction *>({child0, child2, child3}));

  // attempt to take non-existing one
  EXPECT_TRUE(procedure.TakeInstruction(3) == nullptr);
}

TEST_F(ProcedureTest, ConstructedFromString)
{
  // General properties
  ASSERT_NE(loaded_proc.get(), nullptr);
  EXPECT_TRUE(loaded_proc->GetFilename().empty());
  EXPECT_EQ(loaded_proc->GetStatus(), ExecutionStatus::NOT_STARTED);

  // Instruction list
  auto root = loaded_proc->RootInstruction();
  EXPECT_NE(root, nullptr);
  auto instructions = loaded_proc->GetTopInstructions();
  EXPECT_EQ(instructions.size(), 2);

  // Add one instruction
  Instruction *p_wait = wait.get();
  EXPECT_NO_THROW(loaded_proc->PushInstruction(std::move(wait)));
  instructions = loaded_proc->GetTopInstructions();
  EXPECT_EQ(instructions.size(), 3);
  EXPECT_EQ(loaded_proc->RootInstruction(), root);

  // Variables
  auto variables = loaded_proc->VariableNames();
  EXPECT_EQ(variables.size(), 3);
  std::string var1_name = "var1";
  std::string var2_name = "var2";
  std::string var3_name = "var3";
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var3_name), variables.end());
  sup::dto::AnyValue val;
  EXPECT_TRUE(loaded_proc->GetVariableValue(var1_name, val));
}

TEST_F(ProcedureTest, ExternalInclude)
{
  // preparing test file for inclusion
  const std::string external_include_body{R"(
    <Sequence name="MyWait">
        <Wait timeout="$par1" />
        <Wait timeout="$par1" />
    </Sequence>
    <Sequence isRoot="True">
        <Include name="Local Wait Sequence" path="MyWait" par1="1.0" />
        <Include name="External Wait" path="Parallel Wait" file="parallel_sequence.xml" />
    </Sequence>
)"};

  const std::string external_include_file_name = "external_include.xml";
  sup::UnitTestHelper::TemporaryTestFile external_include_file(
      external_include_file_name,
      sup::UnitTestHelper::CreateProcedureString(external_include_body));

  const std::string parallel_sequence_body{R"(
    <ParallelSequence name="Parallel Wait" successThreshold="2">
        <Wait name="One" timeout="1.0" />
        <Wait name="Two" timeout="2.0" />
        <Wait name="Three" timeout="3.0" />
    </ParallelSequence>
    <Workspace>
    </Workspace>
)"};

  const std::string parallel_sequence_file_name = "parallel_sequence.xml";
  sup::UnitTestHelper::TemporaryTestFile parallel_sequence_file(
      parallel_sequence_file_name,
      sup::UnitTestHelper::CreateProcedureString(parallel_sequence_body));

  auto proc = ParseProcedureFile(external_include_file_name);
  ASSERT_TRUE(static_cast<bool>(proc));

  auto proc_context = proc->GetContext();
  auto& sub_proc = proc_context.GetProcedure(parallel_sequence_file_name);
  auto ext_instructions = sub_proc.GetTopInstructions();
  EXPECT_GT(ext_instructions.size(), 0);

  EXPECT_NO_THROW(proc->Setup());
}

TEST_F(ProcedureTest, GetProcedureName)
{
  {
    // Procedure without name attribute or filename returns empty string
    Procedure procedure{};
    EXPECT_EQ(GetProcedureName(procedure), "");
  }
  {
    // Procedure without name attribute returns filename
    const std::string filename = "proc_file";
    Procedure procedure{filename};
    EXPECT_EQ(GetProcedureName(procedure), filename);
  }
  {
    // Procedure with name attribute returns this name
    const std::string filename = "proc_file";
    const std::string name = "proc_name";
    Procedure procedure{filename};
    EXPECT_TRUE(procedure.AddAttribute(Constants::NAME_ATTRIBUTE_NAME, name));
    EXPECT_EQ(GetProcedureName(procedure), name);
  }
  {
    // Procedure with name attribute returns this name, even when empty
    const std::string filename = "proc_file";
    Procedure procedure{filename};
    EXPECT_TRUE(procedure.AddAttribute(Constants::NAME_ATTRIBUTE_NAME, ""));
    EXPECT_EQ(GetProcedureName(procedure), "");
  }
}

TEST_F(ProcedureTest, ProcedureTimingConfigurations)
{
  // Procedure with name attribute returns this name
  const std::string filename = "proc_file";
  const std::string proc_body{
    R"(<?xml version="1.0" encoding="UTF-8"?>
    <Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
      name="Common header"
      xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
      xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd"
      timingAccuracy="0.02">
        <Sequence name="Wait">
            <Wait name="One" timeout="0.3" />
        </Sequence>
        <Workspace>
        </Workspace>
    </Procedure>)"};
  sup::UnitTestHelper::TemporaryTestFile temp_file(
      filename, proc_body);
  Procedure procedure{filename};
  auto proc = ParseProcedureFile(filename);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_NO_THROW(proc->Setup());

  sup::UnitTestHelper::EmptyUserInterface ui;
  ASSERT_TRUE(proc.get() != nullptr);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

ProcedureTest::ProcedureTest()
    : mock_ui{}
    , empty_proc{}
    , loaded_proc{ParseProcedureString(
          sup::UnitTestHelper::CreateProcedureString(ProcedureString))}
    , wait{GlobalInstructionRegistry().Create("Wait")}
    , var1{GlobalVariableRegistry().Create("Local")}
    , var2{GlobalVariableRegistry().Create("Local")}
{
  loaded_proc->Setup();
}

ProcedureTest::~ProcedureTest() = default;
