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

#include "Condition.h"
#include "LocalVariable.h"
#include "SequenceParser.h"
#include "Workspace.h"

#include "LogUI.h"
#include "UnitTestHelper.h"

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

using namespace sup::sequencer;

static const ccs::types::char8 *conditionTable[][14] = {
        { "c", "{\"type\":\"uint8\"}", "0", "c", "false", NULL },
        { "c", "{\"type\":\"uint8\"}", "255", "c", "true", NULL },
        { "c", "{\"type\":\"uint64\"}", "10", "c", "true", NULL },
        { "c.field1", "{\"type\":\"StructuredData1\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}", "{\"field1\":2}", "c", "true", NULL },
        { "c[1].field1", "{\"type\":\"StructuredData6a\", \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}", "[{\"field1\":1}, {\"field1\":0}]", "c", "false", NULL },
        { NULL } };

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);

TEST(Condition, Default) {
  const std::string body{R"(
    <Sequence>
        <Condition name="Condition" var_name="a" />
    </Sequence>
    <Workspace>
        <Local name="a"
               type='{"type":"int8"}'
               value='1' />
        <Local name="b"
               type='{"type":"uint8"}'
               value='0' />
        <Local name="c"
               type='{"type":"uint16"}'
               value='3' />
        <Local name="d"
               type='{"type":"uint32"}'
               value='0' />
    </Workspace>
)"};

  const std::string file_name = "/tmp/workspaceCondition.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);

  ASSERT_TRUE(PrintProcedureWorkspace(proc.get()));

  LogUI ui;
  proc->ExecuteSingle(&ui);
  ASSERT_EQ(proc->GetStatus(), ExecutionStatus::SUCCESS);

  ASSERT_TRUE(PrintProcedureWorkspace(proc.get()));
}

TEST(Condition, Default1) {
  bool status(true);

  ccs::types::uint32 i = 0u;
  while ((conditionTable[i][0] != NULL) && status) {

    std::unique_ptr<Procedure> proc(new Procedure);
    std::unique_ptr<Condition> myCondNode(new Condition);
    myCondNode->AddAttribute("var_name", conditionTable[i][0]);

    ccs::types::AnyValue resVal;

    std::unique_ptr<Variable> varX(new LocalVariable);
    varX->AddAttribute("type", conditionTable[i][1]);
    varX->AddAttribute("value", conditionTable[i][2]);

    varX->GetValue(resVal);
    proc->AddVariable(conditionTable[i][3], varX.release());

    status = PrintProcedureWorkspace(proc.get());

    if (status) {
      LogUI ui;
      proc->PushInstruction(myCondNode.release());
      proc->Setup();
      proc->ExecuteSingle(&ui);

      ::std::string result = conditionTable[i][4];
      if (result == "true") {
        status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
      } else {
        status = (proc->GetStatus() == ExecutionStatus::FAILURE);
      }
    }

    if (status) {
      status = PrintProcedureWorkspace(proc.get());
    }

    i++;
  }

  ASSERT_TRUE(status);
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

TEST(Condition, NonScalarVariable_success) {
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" var_name="struct.timestamp"/>
    </Repeat>
    <Workspace>
        <Local name="struct"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, &ui));
}

TEST(Condition, NonScalarVariable_failure) {
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" var_name="struct"/>
    </Repeat>
    <Workspace>
        <Local name="struct"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(
      proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

TEST(Condition, NoSuchVariable_name) {
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" var_name="undefined"/>
    </Repeat>
    <Workspace>
    </Workspace>
)"};

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(
      proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

TEST(Condition, NoSuchVariable_attr) {
  const std::string body{R"(
    <Repeat maxCount="10">
        <Condition name="struct" var_name="struct.array[0].node"/>
    </Repeat>
    <Workspace>
        <Local name="struct"
               type='{"type":"sup::test::MyType/v1.0","attributes":[{"timestamp":{"type":"uint64"}}]}'
               value='{"timestamp":1}'/>
    </Workspace>"
)"};

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(
      proc, &ui, sup::sequencer::ExecutionStatus::FAILURE));
}

#undef LOG_ALTERN_SRC
