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

#include <gtest/gtest.h>

#include "common/AnyType.h"
#include "common/AnyValue.h"
#include "common/BasicTypes.h"
#include "common/SharedReference.h"
#include <common/log-api.h>

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "LocalVariable.h"
#include "SequenceParser.h"
#include "Workspace.h"

#include "LogUI.h"

#include "UnitTestHelper.h"

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

using namespace sup::sequencer;

// Function definition
static const ccs::types::char8 *testTable[][2] = {{"{\"type\":\"uint8\"}", "0"},
                                                  {"{\"type\":\"uint8\"}", "1"},
                                                  {"{\"type\":\"uint8\"}", "2"},
                                                  {"{\"type\":\"uint8\"}", "3"},
                                                  {NULL}};
static ::ccs::types::uint8 resVal[] = {1, 2, 3, 0};

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);

TEST(Choice, Default) // Static initialisation
{
  bool status(true);
  ccs::types::uint32 i = 0u;
  while ((testTable[i][0] != NULL) && status) {
    auto proc =
        sup::sequencer::ParseProcedureFile("../resources/workspace_choice.xml");

    std::unique_ptr<Variable> varX(new LocalVariable);

    varX->AddAttribute("type", testTable[i][0]);
    varX->AddAttribute("value", testTable[i][1]);
    proc->AddVariable("sel", varX.release());

    status = PrintProcedureWorkspace(proc.get());

    if (status) {
      LogUI ui;
      proc->Setup();
      while ((proc->GetStatus() != ExecutionStatus::SUCCESS) &&
             (proc->GetStatus() != ExecutionStatus::FAILURE)) {
        proc->ExecuteSingle(&ui);
      }

      status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
    }

    if (status) {
      ::ccs::types::AnyValue result;
      proc->GetVariableValue("res", result);
      ::ccs::types::uint8 checkVal =
          *(::ccs::types::uint8 *)(result.GetInstance());
      status = checkVal == resVal[i];

      if (!status) {
        printf("Failed %u!=%u\n", checkVal, resVal[i]);
      }
    }

    if (status) {
      status = PrintProcedureWorkspace(proc.get());
    }

    i++;
  }

  ASSERT_EQ(true, status);
}

TEST(Choice, BitMask_success) {
  const std::string body{R"(
    <Choice is_mask="true" var_name="choice">
        <Counter/>
        <Counter incr="2"/>
        <Wait timeout="0.1"/>
        <Inverter>
            <Counter/>
        </Inverter>
    </Choice>
    <Workspace>
        <Local name="choice" type='{"type":"uint32"}' value="7"/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
  ASSERT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 3);
}

TEST(Choice, BitMask_failure) {
  const std::string body{R"(
    <Choice is_mask="true" var_name="choice">
        <Counter/>
        <Counter incr="2"/>
        <Wait timeout="0.1"/>
        <Inverter>
            <Counter/>
        </Inverter>
    </Choice>
    <Workspace>
        <Local name="choice" type='{"type":"uint32"}' value="14"/>
    </Workspace>
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  // Instruction called and return failure
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(
      proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
  ASSERT_EQ(sup::UnitTestHelper::CounterInstruction::GetCount(), 3);
}

TEST(Choice, NoSuchVariable) {
  const std::string body{R"(
    <Choice var_name="undefined">"
        <Counter/>"
        <Wait timeout="0.1"/>"
    </Choice>"
    <Workspace/>"
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  // Expect failure in Setup
  ASSERT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Choice, NoAttribute) {
  const std::string body{R"(
    <Choice>"
        <Counter/>"
        <Wait timeout="0.1"/>"
    </Choice>"
    <Workspace/>"
)"};

  sup::UnitTestHelper::MockUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  // Expect failure in Setup
  ASSERT_FALSE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure) {
  auto var_names = procedure->VariableNames();
  ::ccs::types::char8 val_string[1024];
  for (const auto &var_name : var_names) {
    ::ccs::types::AnyValue val;
    log_debug("Variable '%s'", var_name.c_str());

    bool var_initialized = procedure->GetVariableValue(var_name, val);
    if (var_initialized) {
      val.SerialiseInstance(val_string, 1024);
      log_debug("Variable '%s', with value\n  %s", var_name.c_str(),
                val_string);
    } else {
      log_debug("Variable '%s' uninitialized", var_name.c_str());
    }
  }
  return true;
}

#undef LOG_ALTERN_SRC
