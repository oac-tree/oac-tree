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

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"
#include "SequenceParser.h"
#include "UnitTestHelper.h"
#include "Workspace.h"

#include "LogUI.h"

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

using namespace sup::sequencer;

class CopyNode : public Instruction {
private:
  ExecutionStatus ExecuteSingleImpl(UserInterface *ui, Workspace *ws) override;

public:
  CopyNode();
  ~CopyNode() override = default;

  static const std::string Type;
};

const std::string CopyNode::Type = "CopyNode";

static bool CopyNodeRegistered = RegisterGlobalInstruction<CopyNode>();

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

TEST(SequenceWorkspace, CopyVariable) {
  const std::string body{R"(
    <Sequence>
        <Copy name="Copy workspace variables" input="var1" output="var2" />
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"uint64_struct","attributes":[{"value":{"type":"uint64"}}]}' />
    </Workspace>
)"};

  const std::string file_name = "/tmp/workspace_copyvar.xml";
  ::sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, ::sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = sup::sequencer::ParseProcedureFile(file_name);
  ASSERT_TRUE(proc.get() != nullptr);

  ASSERT_TRUE(PrintProcedureWorkspace(proc.get()));

  LogUI ui;
  proc->ExecuteSingle(&ui);
  EXPECT_EQ(proc->GetStatus(), ExecutionStatus::SUCCESS);

  ASSERT_TRUE(PrintProcedureWorkspace(proc.get()));
}

CopyNode::CopyNode() : Instruction(Type) {}

ExecutionStatus CopyNode::ExecuteSingleImpl(UserInterface *ui, Workspace *ws) {
  ::ccs::types::AnyValue val;

  bool status = HasAttribute("input");

  if (status) {
    std::string var_input = GetAttribute("input");
    log_debug("CopyNode::ExecuteSingleImpl() - read input variable '%s'",
              var_input.c_str());
    status = ws->GetValue(var_input, val);
  }

  if (status) {
    status = HasAttribute("output");
  }

  if (status) {
    std::string var_output = GetAttribute("output");
    log_debug("CopyNode::ExecuteSingleImpl() - write output variable '%s'",
              var_output.c_str());
    status = ws->SetValue(var_output, val);
  }

  if (status) {
    return ExecutionStatus::SUCCESS;
  }

  return ExecutionStatus::FAILURE;
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
