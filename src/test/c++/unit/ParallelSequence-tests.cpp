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

// Type definition

using namespace sup::sequencer;

class CopyInstruction : public Instruction
{
  private:
    /**
     * @brief See sup::sequencer::Instruction.
     * @details Copy variables identified with 'input' and 'output' attributes.
     */
    ExecutionStatus ExecuteSingleImpl (UserInterface * ui, Workspace * ws) override;

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

static const std::string ProcedureSequenceString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Repeat maxCount="10">
        <Sequence>
            <Wait name="wait" timeout="0.1"/>
            <Wait name="again" timeout="0.1"/>
        </Sequence>
    </Repeat>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
</Procedure>
)RAW";

static const std::string ProcedureParallelString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Repeat maxCount="10">
        <ParallelSequence name="parallel" successThreshold="1">
            <Wait name="wait" timeout="1.0"/>
            <Wait name="again" timeout="0.1"/>
        </ParallelSequence>
    </Repeat>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
</Procedure>
)RAW";

static const std::string ProcedureParallelBuiltinString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Repeat maxCount="10">
        <ParallelSequence name="parallel" successThreshold="1">
            <Wait name="wait" timeout="1.0"/>
            <Copy name="copy" input="input" output="output"/>
            <Wait name="again" timeout="1.0"/>
        </ParallelSequence>
    </Repeat>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
</Procedure>
)RAW";

static const std::string ProcedureParallelUserCodeString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Repeat maxCount="10">
        <ParallelSequence name="parallel" successThreshold="1">
            <Wait name="wait" timeout="1.0"/>
            <CopyInstruction name="copy" input="input" output="output"/>
            <Wait name="again" timeout="1.0"/>
        </ParallelSequence>
    </Repeat>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
</Procedure>
)RAW";

// Function definition

ExecutionStatus CopyInstruction::ExecuteSingleImpl(UserInterface * ui, Workspace * ws)
{
  (void)ui;
  (void)ws;

  bool status = (HasAttribute("input") && HasAttribute("output"));
  ccs::types::AnyValue _value;
  if (status)
  { // Read from workspace
    status = ws->GetValue(GetAttribute("input"), _value);
  }
  if (status)
  { // Write to workspace
    status = ws->SetValue(GetAttribute("output"), _value);
  }
  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);
}

CopyInstruction::CopyInstruction (void) : sup::sequencer::Instruction(CopyInstruction::Type) {}
CopyInstruction::~CopyInstruction (void)
{
  log_info("CopyInstruction::~CopyInstruction called");
}

TEST(ParallelSequence, Procedure_sequence)
{
  bool status = true;
  try
  {
    auto proc = sup::sequencer::ParseProcedureString(ProcedureSequenceString);
    if (status)
    {
      status = static_cast<bool>(proc);
    }
    if (status)
    {
      status = proc->Setup();
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
      } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
               (sup::sequencer::ExecutionStatus::FAILURE != exec));
      status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
    }
  }
  catch (const std::exception &e)
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
    auto proc = sup::sequencer::ParseProcedureString(ProcedureParallelString);
    if (status)
    {
      status = static_cast<bool>(proc);
    }
    if (status)
    {
      status = proc->Setup();
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
      } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
               (sup::sequencer::ExecutionStatus::FAILURE != exec));
      status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
      proc->Reset(); // Wait for thread termination before calling destructor of UI
    }
  }
  catch (const std::exception &e)
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
    auto proc = sup::sequencer::ParseProcedureString(ProcedureParallelBuiltinString);
    if (status)
    {
      status = static_cast<bool>(proc);
    }
    if (status)
    {
      status = proc->Setup();
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
      } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
               (sup::sequencer::ExecutionStatus::FAILURE != exec));
      status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
      proc->Reset(); // Wait for thread termination before calling destructor of UI
    }
  }
  catch (const std::exception &e)
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
    auto proc = sup::sequencer::ParseProcedureString(ProcedureParallelUserCodeString);
    if (status)
    {
      status = static_cast<bool>(proc);
    }
    if (status)
    {
      status = proc->Setup();
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
      } while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
               (sup::sequencer::ExecutionStatus::FAILURE != exec));
      status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
      proc->Reset(); // Wait for thread termination before calling destructor of UI
    }
  }
  catch (const std::exception &e)
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
