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

#include "sup/sequencer/execution_status.h"
#include <sup/sequencer/instructions/log_instruction.h>
#include <sup/sequencer/log_severity.h>

#include "unit_test_helper.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/sequence_parser.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/variable_registry.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <cstddef>

using namespace sup::sequencer;

static const std::string BOOLEANTYPE = R"RAW({"type":"bool"})RAW";

static const std::string LOGVARIABLEPROCEDURE = R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Sequence>
        <Copy input="a" output="out"/>
        <Log input="out" severity="critical"/>
        <ResetVariable varName="out"/>
        <Copy input="b" output="out"/>
        <Log input="out" severity="error"/>
        <ResetVariable varName="out"/>
        <Copy input="c" output="out"/>
        <Log input="out" severity="warning"/>
        <ResetVariable varName="out"/>
        <Copy input="d" output="out"/>
        <Log input="out" severity="notice"/>
    </Sequence>
    <Workspace>
        <Local name="a" type='{"type":"int8"}' value='1' />
        <Local name="b" type='{"type":"int8"}' value='2' />
        <Local name="c" type='{"type":"int8"}' value='3' />
        <Local name="d" type='{"type":"int8"}' value='4' />
        <Local name="out" type='{"type":"int8"}'/>
    </Workspace>
</Procedure>)RAW";

//! Testing Message instruction.
class LogInstructionTest : public ::testing::Test
{
public:
  size_t NumberOfLogEntries() const;
  std::pair<int, std::string> LastLogEntry() const;

protected:
  Procedure proc;
  sup::UnitTestHelper::TestLogUserInterface ui;
  Workspace ws;
};

TEST_F(LogInstructionTest, Setup)
{
  Procedure proc;
  auto instruction = GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  std::string log_message = "Hello test!";
  EXPECT_THROW(instruction->Setup(proc), InstructionSetupException);
  EXPECT_TRUE(instruction->AddAttribute("message", log_message));
  EXPECT_NO_THROW(instruction->Setup(proc));
}

TEST_F(LogInstructionTest, SimpleMessage)
{
  Procedure proc;
  auto instruction = sup::sequencer::GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  std::string log_message = "Hello test!";
  EXPECT_TRUE(instruction->AddAttribute("message", log_message));
  EXPECT_NO_THROW(instruction->Setup(proc));

  Workspace ws;
  EXPECT_EQ(NumberOfLogEntries(), 0);
  EXPECT_NO_THROW(instruction->ExecuteSingle(ui, ws));
  EXPECT_EQ(instruction->GetStatus(), ExecutionStatus::SUCCESS);
  EXPECT_EQ(NumberOfLogEntries(), 1);
  auto last_entry = LastLogEntry();
  EXPECT_EQ(last_entry.first, log::SUP_SEQ_LOG_INFO);
  EXPECT_EQ(last_entry.second, log_message);
}

TEST_F(LogInstructionTest, MessageWithSeverity)
{
  auto instruction = sup::sequencer::GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  Procedure proc;
  std::string log_message = "Hello test!";
  EXPECT_TRUE(instruction->AddAttribute("message", log_message));
  EXPECT_TRUE(instruction->AddAttribute("severity", "critical"));
  EXPECT_NO_THROW(instruction->Setup(proc));

  Workspace ws;
  EXPECT_EQ(NumberOfLogEntries(), 0);
  EXPECT_NO_THROW(instruction->ExecuteSingle(ui, ws));
  EXPECT_EQ(instruction->GetStatus(), ExecutionStatus::SUCCESS);
  EXPECT_EQ(NumberOfLogEntries(), 1);
  auto last_entry = LastLogEntry();
  EXPECT_EQ(last_entry.first, log::SUP_SEQ_LOG_CRIT);
  EXPECT_EQ(last_entry.second, log_message);
}

TEST_F(LogInstructionTest, MessageWithSeverityError)
{
  auto instruction = sup::sequencer::GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  Procedure proc;
  std::string log_message = "Hello test!";
  EXPECT_TRUE(instruction->AddAttribute("message", log_message));
  EXPECT_TRUE(instruction->AddAttribute("severity", "superdupercritical"));
  EXPECT_NO_THROW(instruction->Setup(proc));

  Workspace ws;
  EXPECT_EQ(NumberOfLogEntries(), 0);
  EXPECT_NO_THROW(instruction->ExecuteSingle(ui, ws));
  EXPECT_EQ(instruction->GetStatus(), ExecutionStatus::FAILURE);
  EXPECT_EQ(NumberOfLogEntries(), 1);
  auto last_entry = LastLogEntry();
  EXPECT_EQ(last_entry.first, log::SUP_SEQ_LOG_ERR);
  EXPECT_NE(last_entry.second.find("Log"), std::string::npos);
  EXPECT_NE(last_entry.second.find("superdupercritical"), std::string::npos);
}

TEST_F(LogInstructionTest, VariableDoesNotExist)
{
  auto instruction = sup::sequencer::GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  Procedure proc;
  EXPECT_TRUE(instruction->AddAttribute("input", "does_not_exist"));
  EXPECT_NO_THROW(instruction->Setup(proc));

  Workspace ws;
  EXPECT_EQ(NumberOfLogEntries(), 0);
  EXPECT_NO_THROW(instruction->ExecuteSingle(ui, ws));
  EXPECT_EQ(instruction->GetStatus(), ExecutionStatus::FAILURE);
  EXPECT_EQ(NumberOfLogEntries(), 1);
  auto last_entry = LastLogEntry();
  EXPECT_EQ(last_entry.first, log::SUP_SEQ_LOG_ERR);
  EXPECT_NE(last_entry.second.find("Log"), std::string::npos);
  EXPECT_NE(last_entry.second.find("does_not_exist"), std::string::npos);
}

TEST_F(LogInstructionTest, VariableCannotBeRead)
{
  auto instruction = sup::sequencer::GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  Workspace ws;
  auto variable = GlobalVariableRegistry().Create("Local");
  ASSERT_TRUE(static_cast<bool>(variable));
  EXPECT_TRUE(ws.AddVariable("var", std::move(variable)));
  EXPECT_NO_THROW(ws.Setup());

  Procedure proc;
  EXPECT_TRUE(instruction->AddAttribute("input", "var"));
  EXPECT_NO_THROW(instruction->Setup(proc));

  EXPECT_EQ(NumberOfLogEntries(), 0);
  EXPECT_NO_THROW(instruction->ExecuteSingle(ui, ws));
  EXPECT_EQ(instruction->GetStatus(), ExecutionStatus::FAILURE);
  EXPECT_EQ(NumberOfLogEntries(), 1);
  auto last_entry = LastLogEntry();
  EXPECT_EQ(last_entry.first, log::SUP_SEQ_LOG_WARNING);
  EXPECT_NE(last_entry.second.find("Log"), std::string::npos);
  EXPECT_NE(last_entry.second.find("var"), std::string::npos);
}

TEST_F(LogInstructionTest, VariableSuccess)
{
  auto instruction = sup::sequencer::GlobalInstructionRegistry().Create("Log");
  ASSERT_TRUE(static_cast<bool>(instruction));

  Workspace ws;
  auto variable = GlobalVariableRegistry().Create("Local");
  ASSERT_TRUE(static_cast<bool>(variable));
  EXPECT_TRUE(variable->AddAttribute("type", BOOLEANTYPE));
  EXPECT_TRUE(variable->AddAttribute("value", "true"));
  EXPECT_TRUE(ws.AddVariable("var", std::move(variable)));
  EXPECT_NO_THROW(ws.Setup());

  Procedure proc;
  EXPECT_TRUE(instruction->AddAttribute("input", "var"));
  EXPECT_NO_THROW(instruction->Setup(proc));

  EXPECT_EQ(NumberOfLogEntries(), 0);
  EXPECT_NO_THROW(instruction->ExecuteSingle(ui, ws));
  EXPECT_EQ(instruction->GetStatus(), ExecutionStatus::SUCCESS);
  EXPECT_EQ(NumberOfLogEntries(), 1);
  auto last_entry = LastLogEntry();
  EXPECT_EQ(last_entry.first, log::SUP_SEQ_LOG_INFO);
  EXPECT_NE(last_entry.second.find("true"), std::string::npos);
}

TEST_F(LogInstructionTest, ParsedProcedure)
{
  auto proc = ParseProcedureString(LOGVARIABLEPROCEDURE);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_NO_THROW(proc->Setup());

  ExecutionStatus exec = ExecutionStatus::FAILURE;
  do
  {
    proc->ExecuteSingle(ui);
    exec = proc->GetStatus();

  } while ((ExecutionStatus::SUCCESS != exec) && ExecutionStatus::FAILURE != exec);

  EXPECT_EQ(exec, ExecutionStatus::SUCCESS);
  EXPECT_EQ(NumberOfLogEntries(), 4);
}

size_t LogInstructionTest::NumberOfLogEntries() const
{
  return ui.m_log_entries.size();
}

std::pair<int, std::string> LogInstructionTest::LastLogEntry() const
{
  return ui.m_log_entries.back();
}
