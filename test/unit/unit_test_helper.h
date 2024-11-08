/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_UNIT_TEST_HELPER_H_
#define SUP_SEQUENCER_UNIT_TEST_HELPER_H_

#include <sup/sequencer/async_input_adapter.h>
#include <sup/sequencer/compound_instruction.h>
#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/i_job_info_io.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

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
class CounterInstruction : public sup::sequencer::Instruction
{
public:
  CounterInstruction();
  ~CounterInstruction() override;

  static const std::string Type;

  static unsigned long counter;
  static unsigned long GetCount();

private:
  sup::sequencer::ExecutionStatus ExecuteSingleImpl(sup::sequencer::UserInterface& ui,
                                                    sup::sequencer::Workspace& ws) override;
};

class TestTreeInstruction : public sup::sequencer::CompoundInstruction
{
public:
  TestTreeInstruction();
  ~TestTreeInstruction() override;

  static const std::string Type;

private:
  sup::sequencer::ExecutionStatus ExecuteSingleImpl(sup::sequencer::UserInterface& ui,
                                                    sup::sequencer::Workspace& ws) override;
};

std::unique_ptr<sup::sequencer::Instruction> CreateTestTreeInstruction(
  const std::string& name = {});

class MockUI : public sup::sequencer::DefaultUserInterface
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

  /**
   * @brief See sup::sequencer::UserInterface.
   */
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description) override;
  int GetUserChoice(const std::vector<std::string>& options,
                    const sup::dto::AnyValue& metadata) override;
  std::unique_ptr<sup::sequencer::IUserInputFuture> RequestUserInput(
    const sup::sequencer::UserInputRequest& request) override;

private:
  sup::sequencer::UserInputReply UserInput(const sup::sequencer::UserInputRequest& request,
                                           sup::dto::uint64 id);
  void Interrupt(sup::dto::uint64 id);
  sup::sequencer::AsyncInputAdapter m_input_adapter;
  bool m_status = false;
  int m_choice = -1;
  sup::dto::AnyValue m_value;
  std::vector<std::string> m_options;
  std::unique_ptr<sup::dto::AnyValue> m_metadata;
};

using EmptyUserInterface = sup::sequencer::DefaultUserInterface;

class TestLogUserInterface : public sup::sequencer::DefaultUserInterface
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

class MockJobInfoIO : public sup::sequencer::IJobInfoIO
{
public:
  MOCK_METHOD(void, InitNumberOfInstructions, (sup::dto::uint32), (override));
  MOCK_METHOD(void, InstructionStateUpdated, (sup::dto::uint32, sup::sequencer::InstructionState), (override));
  MOCK_METHOD(void, VariableUpdated, (sup::dto::uint32, const sup::dto::AnyValue&, bool), (override));
  MOCK_METHOD(void, JobStateUpdated, (sup::sequencer::JobState), (override));
  MOCK_METHOD(void, PutValue, (const sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(bool, GetUserValue, (sup::dto::AnyValue&, const std::string&), (override));
  MOCK_METHOD(int, GetUserChoice, (const std::vector<std::string>&, const sup::dto::AnyValue&), (override));
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
    std::unique_ptr<sup::sequencer::Procedure>& proc, sup::sequencer::UserInterface& ui,
    const sup::sequencer::ExecutionStatus& expect = sup::sequencer::ExecutionStatus::SUCCESS);

static inline bool TryAndExecuteNoReset(std::unique_ptr<sup::sequencer::Procedure>& proc,
                                        sup::sequencer::UserInterface& ui,
                                        const sup::sequencer::ExecutionStatus& expect)
{
  bool status = static_cast<bool>(proc);

  proc->Setup();

  if (status)
  {
    sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

    do
    {
      if (exec == sup::sequencer::ExecutionStatus::RUNNING)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      proc->ExecuteSingle(ui);
      exec = proc->GetStatus();
    } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec)
             && (sup::sequencer::ExecutionStatus::FAILURE != exec));

    status = (expect == exec);
  }

  return status;
}

static inline bool TryAndExecute(std::unique_ptr<sup::sequencer::Procedure>& proc,
                                 sup::sequencer::UserInterface& ui,
                                 const sup::sequencer::ExecutionStatus& expect)
{

  bool status = TryAndExecuteNoReset(proc, ui, expect);
  if (proc)
  {
    proc->Teardown(ui);
  }
  return status;
}

/**
 * Creates a string representing a valid XML of sequencer procedure by enclosing user provided body
 * between appropriate header and footer.
 */
std::string CreateProcedureString(const std::string& body);

/**
 * Prints variables in a workspace.
 */
void PrintProcedureWorkspace(sup::sequencer::Procedure* procedure);


/**
 * Returns text file content.
 */
std::string GetTextFileContent(const std::string &file_name);

}  // namespace UnitTestHelper

}  // namespace sup

#endif  // SUP_SEQUENCER_UNIT_TEST_HELPER_H_
