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

#include <common/AnyValue.h>

#include "InstructionRegistry.h"
#include "SequenceParser.h"
#include "UnitTestHelper.h"

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

TEST(SequenceParser, Default) // Static initialisation
{
  const std::string body{R"(
    <Sequence>
        <Wait name="First" />
        <Wait name="Second" />
        <Inverter>
            <Wait name="Third" />
        </Inverter>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  const std::string file_name = "/tmp/sequence_fail.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);
}

TEST(SequenceParser, Workspace) // Static initialisation
{
  const std::string body{R"(
    <Sequence>
        <Wait name="Only" />
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"vartype1","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"vartype2","attributes":[{"value":{"type":"string"}}]}' />
        <Local name="var3"
               type='{"type":"vartype3","attributes":[{"value":{"type":"bool"}}]}'/>
    </Workspace>
)"};

  const std::string file_name = "/tmp/workspace.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(PrintProcedureWorkspace(proc.get()));
}

TEST(SequenceParser, ParseString) // Static initialisation
{
  const std::string body{R"(
    <Sequence>
        <Wait name="First" />
        <Wait name="Second" />
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  auto proc = sup::sequencer::ParseProcedureString(
      ::sup::UnitTestHelper::CreateProcedureString(body));

  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(PrintProcedureWorkspace(proc.get()));
}

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure) {
  auto var_names = procedure->VariableNames();
  ::ccs::types::char8 val_string[1024];
  for (const auto &var_name : var_names) {
    ::ccs::types::AnyValue val;
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
