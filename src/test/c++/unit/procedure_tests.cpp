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

#include <sup/sequencer/procedure.h>

#include "mock_user_interface.h"
#include "unit_test_helper.h"

#include <sup/sequencer/variables/local_variable.h>
#include <sup/sequencer/instructions/wait.h>

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/variable.h>
#include <sup/sequencer/variable_registry.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <thread>

using namespace sup::sequencer;

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
  std::string filename = "TestFile.xml";
  EXPECT_NO_THROW(empty_proc.SetFilename(filename));
  EXPECT_EQ(empty_proc.GetFilename(), filename);

  // Empty instruction list
  EXPECT_EQ(empty_proc.RootInstruction(), nullptr);
  auto instructions = empty_proc.GetInstructions();
  EXPECT_EQ(instructions.size(), 0);
  EXPECT_EQ(empty_proc.GetInstructionCount(), 0);

  // Add one instruction
  EXPECT_FALSE(empty_proc.PushInstruction(nullptr));
  Instruction *p_wait = wait.get();
  EXPECT_TRUE(empty_proc.PushInstruction(wait.release()));
  EXPECT_EQ(empty_proc.RootInstruction(), p_wait);
  instructions = empty_proc.GetInstructions();
  EXPECT_EQ(instructions.size(), 1);
  instructions = empty_proc.GetInstructions(filename);
  EXPECT_EQ(instructions.size(), 1);
  std::string wrong_filename = "WrongFile.xml";
  instructions = empty_proc.GetInstructions(wrong_filename);
  EXPECT_EQ(instructions.size(), 0);

  // Variables
  auto variables = empty_proc.VariableNames();
  EXPECT_EQ(variables.size(), 0);
  std::string var1_name = "var1";
  std::string var2_name = "var2";
  EXPECT_TRUE(empty_proc.AddVariable(var1_name, var1.release()));
  variables = empty_proc.VariableNames();
  EXPECT_EQ(variables.size(), 1);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_TRUE(empty_proc.AddVariable(var2_name, var2.release()));
  variables = empty_proc.VariableNames();
  EXPECT_EQ(variables.size(), 2);
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
}

TEST_F(ProcedureTest, GetWorkspace)
{
  Procedure procedure;
  auto variable = GlobalVariableRegistry().Create("Local");
  std::vector<const Variable*> expected({variable.get()});
  procedure.AddVariable("var", variable.release());

  EXPECT_EQ(procedure.GetWorkspace()->GetVariables(), expected);
}

TEST_F(ProcedureTest, InsertInstruction)
{
  Procedure procedure;

  // inserting children one after another
  auto child0 = new Wait;
  EXPECT_TRUE(procedure.InsertInstruction(child0, 0));
  EXPECT_EQ(procedure.GetInstructionCount(), 1);

  auto child1 = new Wait;
  EXPECT_TRUE(procedure.InsertInstruction(child1, 1));
  EXPECT_EQ(procedure.GetInstructionCount(), 2);

  // inserting child in between
  auto child2 = new Wait;
  EXPECT_TRUE(procedure.InsertInstruction(child2, 1));
  EXPECT_EQ(procedure.GetInstructionCount(), 3);

  EXPECT_EQ(procedure.GetInstructions(),
            std::vector<const Instruction *>({child0, child2, child1}));

  // wrong insert index
  Wait child3;
  EXPECT_FALSE(procedure.InsertInstruction(&child3, -1));
  EXPECT_FALSE(procedure.InsertInstruction(&child3, 4));
}

TEST_F(ProcedureTest, TakeMiddleChild)
{
  Procedure procedure;

  // inserting 4 children
  auto child0 = new Wait;
  procedure.InsertInstruction(child0, 0);
  auto child1 = new Wait;
  procedure.InsertInstruction(child1, 1);
  auto child2 = new Wait;
  procedure.InsertInstruction(child2, 2);
  auto child3 = new Wait;
  procedure.InsertInstruction(child3, 3);

  // taking middle child
  auto child1_taken = procedure.TakeInstruction(1);
  EXPECT_EQ(child1, child1_taken);
  EXPECT_EQ(procedure.GetInstructions().size(), 3);
  EXPECT_EQ(procedure.GetInstructions(),
            std::vector<const Instruction *>({child0, child2, child3}));
  delete child1_taken;

  // attempt to take non-existing one
  EXPECT_TRUE(procedure.TakeInstruction(3) == nullptr);
}

TEST_F(ProcedureTest, ConstructedFromString)
{
  // General properties
  ASSERT_NE(loaded_proc.get(), nullptr);
  EXPECT_TRUE(loaded_proc->GetFilename().empty());
  EXPECT_EQ(loaded_proc->GetStatus(), ExecutionStatus::NOT_STARTED);
  std::string filename = "TestFile.xml";
  EXPECT_NO_THROW(loaded_proc->SetFilename(filename));
  EXPECT_EQ(loaded_proc->GetFilename(), filename);

  // Instruction list
  auto root = loaded_proc->RootInstruction();
  EXPECT_NE(root, nullptr);
  auto instructions = loaded_proc->GetInstructions();
  EXPECT_EQ(instructions.size(), 2);

  // Add one instruction
  Instruction *p_wait = wait.get();
  EXPECT_TRUE(loaded_proc->PushInstruction(wait.release()));
  instructions = loaded_proc->GetInstructions();
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

  auto ext_instructions = proc->GetInstructions(parallel_sequence_file_name);
  EXPECT_GT(ext_instructions.size(), 0);

  EXPECT_TRUE(proc->Setup());
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

ProcedureTest::~ProcedureTest()
{
  if (loaded_proc)
  {
    loaded_proc->Reset();
  }
}
