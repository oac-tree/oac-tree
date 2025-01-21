/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_UNIT_TEST_HELPER_H_
#define SUP_OAC_TREE_UNIT_TEST_HELPER_H_

#include <sup/oac-tree/async_input_adapter.h>
#include <sup/oac-tree/compound_instruction.h>
#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/i_job_info_io.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>

#include <chrono>
#include <string>
#include <thread>

const std::string kWorkspaceSequenceBody{
R"RAW(
  <Sequence>
    <Copy inputVar="one" outputVar="var1"/>
    <Copy inputVar="one" outputVar="var2"/>
  </Sequence>
  <Workspace>
    <Local name="one" type='{"type":"uint32"}' value='1'/>
    <Local name="var1" type='{"type":"uint32"}' value='0'/>
    <Local name="var2" type='{"type":"uint32"}' value='0'/>
  </Workspace>
)RAW"};

namespace sup
{
namespace UnitTestHelper
{
class CounterInstruction : public sup::oac_tree::Instruction
{
public:
  CounterInstruction();
  ~CounterInstruction() override;

  static const std::string Type;

  static unsigned long counter;
  static unsigned long GetCount();

private:
  sup::oac_tree::ExecutionStatus ExecuteSingleImpl(sup::oac_tree::UserInterface& ui,
                                                    sup::oac_tree::Workspace& ws) override;
};

class TestTreeInstruction : public sup::oac_tree::CompoundInstruction
{
public:
  TestTreeInstruction();
  ~TestTreeInstruction() override;

  static const std::string Type;

private:
  sup::oac_tree::ExecutionStatus ExecuteSingleImpl(sup::oac_tree::UserInterface& ui,
                                                    sup::oac_tree::Workspace& ws) override;
};

std::unique_ptr<sup::oac_tree::Instruction> CreateTestTreeInstruction(
  const std::string& name = {});

class MockUI : public sup::oac_tree::DefaultUserInterface
{
public:
  MockUI();
  virtual ~MockUI();

  sup::dto::AnyType GetType() const;

  void SetChoice(int choice);
  void SetStatus(bool status);
  void SetValue(sup::dto::AnyValue& value);

  std::vector<std::string> GetOptions() const;
  const sup::dto::AnyValue* GetMetadata() const;

  std::unique_ptr<sup::oac_tree::IUserInputFuture> RequestUserInput(
    const sup::oac_tree::UserInputRequest& request) override;

private:
  sup::oac_tree::UserInputReply UserInput(const sup::oac_tree::UserInputRequest& request,
                                           sup::dto::uint64 id);
  void Interrupt(sup::dto::uint64 id);
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description);
  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata);
  sup::oac_tree::AsyncInputAdapter m_input_adapter;
  bool m_status = false;
  int m_choice = -1;
  sup::dto::AnyValue m_value;
  std::vector<std::string> m_options;
  std::unique_ptr<sup::dto::AnyValue> m_metadata;
};

using EmptyUserInterface = sup::oac_tree::DefaultUserInterface;

class TestLogUserInterface : public sup::oac_tree::DefaultUserInterface
{
public:
  using LogEntry = std::pair<int, std::string>;

  TestLogUserInterface() = default;

  void Log(int severity, const std::string& message) override
  {
    m_log_entries.emplace_back(severity, message);
  }

  std::vector<LogEntry> m_log_entries;
};

class MockJobInfoIO : public sup::oac_tree::IJobInfoIO
{
public:
  MOCK_METHOD(void, InitNumberOfInstructions, (sup::dto::uint32), (override));
  MOCK_METHOD(void, InstructionStateUpdated, (sup::dto::uint32, sup::oac_tree::InstructionState), (override));
  MOCK_METHOD(void, VariableUpdated, (sup::dto::uint32, const sup::dto::AnyValue&, bool), (override));
  MOCK_METHOD(void, JobStateUpdated, (sup::oac_tree::JobState), (override));
  MOCK_METHOD(void, PutValue, (const sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(bool, GetUserValue, (sup::dto::uint64, sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(int, GetUserChoice, (sup::dto::uint64, const std::vector<std::string>&, const sup::dto::AnyValue&), (override));
  MOCK_METHOD(void, Interrupt, (sup::dto::uint64), (override));
  MOCK_METHOD(void, Message, (const std::string&), (override));
  MOCK_METHOD(void, Log, (int, const std::string&), (override));
  MOCK_METHOD(void, NextInstructionsUpdated, (const std::vector<sup::dto::uint32>&), (override));
};

class TemporaryTestFile
{
private:
  std::string filename;

public:
  TemporaryTestFile(std::string filename, std::string contents);
  ~TemporaryTestFile();
};

std::string GetFullTestFilePath(const std::string& filename);

static inline bool TryAndExecute(
    std::unique_ptr<sup::oac_tree::Procedure>& proc, sup::oac_tree::UserInterface& ui,
    const sup::oac_tree::ExecutionStatus& expect = sup::oac_tree::ExecutionStatus::SUCCESS);

static inline bool TryAndExecuteNoReset(std::unique_ptr<sup::oac_tree::Procedure>& proc,
                                        sup::oac_tree::UserInterface& ui,
                                        const sup::oac_tree::ExecutionStatus& expect)
{
  bool status = static_cast<bool>(proc);

  proc->Setup();

  if (status)
  {
    sup::oac_tree::ExecutionStatus exec = sup::oac_tree::ExecutionStatus::FAILURE;

    do
    {
      if (exec == sup::oac_tree::ExecutionStatus::RUNNING)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      proc->ExecuteSingle(ui);
      exec = proc->GetStatus();
    } while ((sup::oac_tree::ExecutionStatus::SUCCESS != exec)
             && (sup::oac_tree::ExecutionStatus::FAILURE != exec));

    status = (expect == exec);
  }

  return status;
}

static inline bool TryAndExecute(std::unique_ptr<sup::oac_tree::Procedure>& proc,
                                 sup::oac_tree::UserInterface& ui,
                                 const sup::oac_tree::ExecutionStatus& expect)
{

  bool status = TryAndExecuteNoReset(proc, ui, expect);
  if (proc)
  {
    proc->Reset(ui);
  }
  return status;
}

/**
 * Creates a string representing a valid XML of oac-tree procedure by enclosing user provided body
 * between appropriate header and footer.
 */
std::string CreateProcedureString(const std::string& body);

/**
 * Prints variables in a workspace.
 */
void PrintProcedureWorkspace(sup::oac_tree::Procedure* procedure);


/**
 * Returns text file content.
 */
std::string GetTextFileContent(const std::string &file_name);

}  // namespace UnitTestHelper

}  // namespace sup

#endif  // SUP_OAC_TREE_UNIT_TEST_HELPER_H_
