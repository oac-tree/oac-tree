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

#ifndef SUP_SEQUENCER_UNIT_TEST_HELPER_H_
#define SUP_SEQUENCER_UNIT_TEST_HELPER_H_

#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anyvalue.h>

#include <chrono>
#include <string>
#include <thread>

namespace sup
{
namespace UnitTestHelper
{
class CounterInstruction : public sup::sequencer::Instruction
{
private:
  /**
   * @brief See sup::sequencer::Instruction.
   */
  sup::sequencer::ExecutionStatus ExecuteSingleImpl(sup::sequencer::UserInterface* ui,
                                                    sup::sequencer::Workspace* ws) override;

protected:
public:
  /**
   * @brief Constructor.
   */
  CounterInstruction();

  /**
   * @brief Destructor.
   */
  ~CounterInstruction() override;

  /**
   * @brief Class name for InstructionRegistry.
   */
  static const std::string Type;

  static unsigned long counter;
  static unsigned long GetCount();
};

class MockUI : public sup::sequencer::UserInterface
{
private:
  bool m_status = false;
  int m_choice = -1;
  sup::dto::AnyValue m_value;

  /**
   * @brief See sup::sequencer::UserInterface.
   */
  void UpdateInstructionStatusImpl(const sup::sequencer::Instruction* instruction) override;
  bool GetUserValueImpl(sup::dto::AnyValue& value, const std::string& description) override;
  int GetUserChoiceImpl(const std::vector<std::string>& choices,
                        const std::string& description) override;

protected:
public:
  sup::dto::AnyType GetType() const;

  void SetChoice(int choice);
  void SetStatus(bool status);
  void SetValue(sup::dto::AnyValue& value);

  MockUI();
  virtual ~MockUI();
};

class EmptyUserInterface : public sup::sequencer::UserInterface
{
private:
  void UpdateInstructionStatusImpl(const sup::sequencer::Instruction* instruction) override {}
public:
  EmptyUserInterface() = default;
  ~EmptyUserInterface() =default;
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
    std::unique_ptr<sup::sequencer::Procedure>& proc, sup::sequencer::UserInterface* const ui,
    const sup::sequencer::ExecutionStatus& expect = sup::sequencer::ExecutionStatus::SUCCESS);

static inline bool TryAndExecuteNoReset(std::unique_ptr<sup::sequencer::Procedure>& proc,
                                        sup::sequencer::UserInterface* const ui,
                                        const sup::sequencer::ExecutionStatus& expect)
{
  bool status = static_cast<bool>(proc);

  if (status)
  {  // Setup procedure
    status = proc->Setup();
  }

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
                                 sup::sequencer::UserInterface* const ui,
                                 const sup::sequencer::ExecutionStatus& expect)
{

  bool status = TryAndExecuteNoReset(proc, ui, expect);
  if (proc)
  {
    proc->Reset();
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
