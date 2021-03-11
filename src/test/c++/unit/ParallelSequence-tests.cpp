/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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

#include <SequenceParser.h>

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"

#include "Workspace.h"

#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

class CopyInstruction : public sup::sequencer::Instruction
{

  private:

    /**
     * @brief See sup::sequencer::Instruction.
     * @details Copy variables identified with 'input' and 'output' attributes.
     */

    sup::sequencer::ExecutionStatus ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    CopyInstruction (void);

    /**
     * @brief Destructor.
     */

    ~CopyInstruction (void) override;

    /**
     * @brief Class name for InstructionRegistry.
     */

    static const std::string Type;

};

// Function declaration

// Global variables

const std::string CopyInstruction::Type = "CopyInstruction";

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

sup::sequencer::ExecutionStatus CopyInstruction::ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = (sup::sequencer::Instruction::HasAttribute("input") && sup::sequencer::Instruction::HasAttribute("output"));

  ccs::types::AnyValue _value;

  if (status)
    { // Read from workspace
      status = ws->GetValue(sup::sequencer::Instruction::GetAttribute("input"), _value);
    }

  if (status)
    { // Write to workspace
      status = ws->SetValue(sup::sequencer::Instruction::GetAttribute("output"), _value);
    }

  return (status ? sup::sequencer::ExecutionStatus::SUCCESS : sup::sequencer::ExecutionStatus::FAILURE);

}

CopyInstruction::CopyInstruction (void) : sup::sequencer::Instruction("CopyInstruction") {}
CopyInstruction::~CopyInstruction (void) {}

TEST(ParallelSequence, Procedure_sequence)
{

  bool status = true;

  try
    {
      auto proc = sup::sequencer::ParseProcedureString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
        "           name=\"Trivial procedure for testing purposes\"\n"
        "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
        "    <Repeat maxCount=\"10\">\n"
        "        <Sequence>\n"
        "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
        "            <Wait name=\"again\" timeout=\"0.1\"/>\n"
        "        </Sequence>\n"
        "    </Repeat>\n"
        "    <Workspace>\n"
        "        <Local name=\"input\" type='{\"type\":\"string\"}' value='\"undefined\"'/>\n"
        "        <Local name=\"output\" type='{\"type\":\"string\"}' value='\"denifednu\"'/>\n"
        "    </Workspace>\n"
        "</Procedure>");

      if (status)
        {
          status = static_cast<bool>(proc);
        }

      if (status)
        {
          sup::sequencer::LogUI ui;
          sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

          do
            {
              (void)ccs::HelperTools::SleepFor(10000000ul); // Let system breathe
              proc->ExecuteSingle(&ui);
              exec = proc->GetStatus();
            }
          while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
                 (sup::sequencer::ExecutionStatus::FAILURE != exec));

          status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
        }
    }
  catch (const std::exception& e)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_sequence) - .. '%s' exception caught", e.what());
      status = false;
    }
  catch (...)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_sequence) - .. unknown exception caught");
      status = false;
    }

  ASSERT_EQ(true, status);

}

TEST(ParallelSequence, Procedure_parallel)
{

  bool status = true;

  try
    {
      auto proc = sup::sequencer::ParseProcedureString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
        "           name=\"Trivial procedure for testing purposes\"\n"
        "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
        "    <Repeat maxCount=\"10\">\n"
        "        <ParallelSequence>\n"
        "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
        "            <Wait name=\"again\" timeout=\"0.1\"/>\n"
        "        </ParallelSequence>\n"
        "    </Repeat>\n"
        "    <Workspace>\n"
        "        <Local name=\"input\" type='{\"type\":\"string\"}' value='\"undefined\"'/>\n"
        "        <Local name=\"output\" type='{\"type\":\"string\"}' value='\"denifednu\"'/>\n"
        "    </Workspace>\n"
        "</Procedure>");

      if (status)
        {
          status = static_cast<bool>(proc);
        }

      if (status)
        {
          sup::sequencer::LogUI ui;
          sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

          do
            {
              (void)ccs::HelperTools::SleepFor(10000000ul); // Let system breathe
              proc->ExecuteSingle(&ui);
              exec = proc->GetStatus();
            }
          while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
                 (sup::sequencer::ExecutionStatus::FAILURE != exec));

          status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
        }
    }
  catch (const std::exception& e)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_parallel) - .. '%s' exception caught", e.what());
      status = false;
    }
  catch (...)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_parallel) - .. unknown exception caught");
      status = false;
    }

  ASSERT_EQ(true, status);

}
// Issue during the tear-down process
TEST(ParallelSequence, WithBuiltinCode)
{

  bool status = true;

  try
    {
      auto proc = sup::sequencer::ParseProcedureString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
        "           name=\"Trivial procedure for testing purposes\"\n"
        "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
        "    <Repeat maxCount=\"10\">\n"
        "        <ParallelSequence>\n"
        "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
        "            <CopyInstruction name=\"copy\" input=\"input\" output=\"output\"/>\n"
        "            <Wait name=\"again\" timeout=\"0.1\"/>\n"
        "        </ParallelSequence>\n"
        "    </Repeat>\n"
        "    <Workspace>\n"
        "        <Local name=\"input\" type='{\"type\":\"string\"}' value='\"undefined\"'/>\n"
        "        <Local name=\"output\" type='{\"type\":\"string\"}' value='\"denifednu\"'/>\n"
        "    </Workspace>\n"
        "</Procedure>");

      if (status)
        {
          status = static_cast<bool>(proc);
        }

      if (status)
        {
          sup::sequencer::LogUI ui;
          sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

          do
            {
              (void)ccs::HelperTools::SleepFor(10000000ul); // Let system breathe
              proc->ExecuteSingle(&ui);
              exec = proc->GetStatus();
            }
          while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
                 (sup::sequencer::ExecutionStatus::FAILURE != exec));

          status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
        }
    }
  catch (const std::exception& e)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_parallel) - .. '%s' exception caught", e.what());
      status = false;
    }
  catch (...)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_parallel) - .. unknown exception caught");
      status = false;
    }

  ASSERT_EQ(true, status);

}
// Issue during the tear-down process
TEST(ParallelSequence, WithUserCode)
{

  bool status = sup::sequencer::RegisterGlobalInstruction<CopyInstruction>();

  try
    {
      auto proc = sup::sequencer::ParseProcedureString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
        "           name=\"Trivial procedure for testing purposes\"\n"
        "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
        "    <Repeat maxCount=\"10\">\n"
        "        <ParallelSequence>\n"
        "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
        "            <Copy name=\"copy\" input=\"input\" output=\"output\"/>\n"
        "            <Wait name=\"again\" timeout=\"0.1\"/>\n"
        "        </ParallelSequence>\n"
        "    </Repeat>\n"
        "    <Workspace>\n"
        "        <Local name=\"input\" type='{\"type\":\"string\"}' value='\"undefined\"'/>\n"
        "        <Local name=\"output\" type='{\"type\":\"string\"}' value='\"denifednu\"'/>\n"
        "    </Workspace>\n"
        "</Procedure>");

      if (status)
        {
          status = static_cast<bool>(proc);
        }

      if (status)
        {
          sup::sequencer::LogUI ui;
          sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

          do
            {
              (void)ccs::HelperTools::SleepFor(10000000ul); // Let system breathe
              proc->ExecuteSingle(&ui);
              exec = proc->GetStatus();
            }
          while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
                 (sup::sequencer::ExecutionStatus::FAILURE != exec));

          status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
        }
    }
  catch (const std::exception& e)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_parallel) - .. '%s' exception caught", e.what());
      status = false;
    }
  catch (...)
    { // Exception caught
      log_error("TEST(ParallelSequence, Procedure_parallel) - .. unknown exception caught");
      status = false;
    }

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
