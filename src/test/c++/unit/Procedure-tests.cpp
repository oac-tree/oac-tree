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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files

#include <gtest/gtest.h> // Google test framework
#include <common/log-api.h> // Syslog wrapper routines
#include <algorithm>
#include <chrono>
#include <thread>

// Local header files

#include "Procedure.h"
#include "InstructionRegistry.h"
#include "MockUserInterface.h"
#include "SequenceParser.h"
#include "UnitTestHelper.h"
#include "Variable.h"
#include "VariableRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

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

// Function declaration

// Global variables

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

static const std::string ProcedureString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
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
</Procedure>
)RAW";

// Function definition

using ::testing::AtLeast;
using ::testing::_;
using ::testing::InSequence;

TEST_F(ProcedureTest, DefaultConstructed)
{
  // Test constructed
  EXPECT_TRUE(empty_proc.GetFilename().empty());
  EXPECT_EQ(empty_proc.GetStatus(), ExecutionStatus::NOT_STARTED);
  std::string filename = "TestFile.xml";
  EXPECT_NO_THROW(empty_proc.SetFilename(filename));
  EXPECT_EQ(empty_proc.GetFilename(), filename);

  // Empty instruction list
  EXPECT_EQ(empty_proc.RootInstrunction(), nullptr);
  auto instructions = empty_proc.GetInstructions();
  EXPECT_EQ(instructions.size(), 0);

  // Add one instruction
  EXPECT_FALSE(empty_proc.PushInstruction(nullptr));
  Instruction *p_wait = wait.get();
  EXPECT_TRUE(empty_proc.PushInstruction(wait.release()));
  EXPECT_EQ(empty_proc.RootInstrunction(), p_wait);
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
  auto root = loaded_proc->RootInstrunction();
  EXPECT_NE(root, nullptr);
  auto instructions = loaded_proc->GetInstructions();
  EXPECT_EQ(instructions.size(), 2);

  // Add one instruction
  Instruction *p_wait = wait.get();
  EXPECT_TRUE(loaded_proc->PushInstruction(wait.release()));
  instructions = loaded_proc->GetInstructions();
  EXPECT_EQ(instructions.size(), 3);
  EXPECT_EQ(loaded_proc->RootInstrunction(), root);

  // Variables
  auto variables = loaded_proc->VariableNames();
  EXPECT_EQ(variables.size(), 3);
  std::string var1_name = "var1";
  std::string var2_name = "var2";
  std::string var3_name = "var3";
  EXPECT_NE(std::find(variables.begin(), variables.end(), var1_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var2_name), variables.end());
  EXPECT_NE(std::find(variables.begin(), variables.end(), var3_name), variables.end());
  ::ccs::types::AnyValue val;
  EXPECT_TRUE(loaded_proc->GetVariableValue(var1_name, val));
}

TEST_F(ProcedureTest, ExternalInclude)
{
  std::string main_file = ::sup::UnitTestHelper::GetFullTestFilePath("external_include.xml");
  auto proc = ParseProcedureFile(main_file);
  ASSERT_TRUE(static_cast<bool>(proc));

  std::string external_file = ::sup::UnitTestHelper::GetFullTestFilePath("parallel_sequence.xml");
  auto ext_instructions = proc->GetInstructions(external_file);
  EXPECT_GT(ext_instructions.size(), 0);

  EXPECT_TRUE(proc->Setup());
}

ProcedureTest::ProcedureTest()
  : mock_ui{}
  , empty_proc{}
  , loaded_proc{ParseProcedureString(ProcedureString)}
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

#undef LOG_ALTERN_SRC
