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

#include <common/BasicTypes.h>

#include <SequenceParser.h>

#include <Instruction.h>
#include <InstructionRegistry.h>

// Local header files

#include "UnitTestHelper.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

class CounterInstruction : public sup::sequencer::Instruction
{

  private:

    /**
     * @brief See sup::sequencer::Instruction.
     */

    sup::sequencer::ExecutionStatus ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    CounterInstruction (void);

    /**
     * @brief Destructor.
     */

    ~CounterInstruction (void) override;

    /**
     * @brief Class name for InstructionRegistry.
     */

    static const std::string Type;

    static ccs::types::uint32 counter;

};

// Function declaration

// Global variables

const std::string CounterInstruction::Type = "Counter";
ccs::types::uint32 CounterInstruction::counter = 0u;

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();
static bool _initialise_instruction = sup::sequencer::RegisterGlobalInstruction<CounterInstruction>();

// Function definition

sup::sequencer::ExecutionStatus CounterInstruction::ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws) { counter++; return sup::sequencer::ExecutionStatus::SUCCESS; }
CounterInstruction::CounterInstruction() : sup::sequencer::Instruction(CounterInstruction::Type) { counter = 0u; }
CounterInstruction::~CounterInstruction() { counter = 0u; }

// ToDo - Should implement test-specific instruction class to verify if called multiple times, or called by the ForceSuccess, etc.
//        Need for this a wait to get a reference to the instruction in the procedure.

TEST(Fallback, Procedure_first)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Fallback>\n"
    "        <ForceSuccess name=\"success\">\n"
    "            <Counter/>\n"
    "        </ForceSuccess>\n"
    "        <ForceSuccess name=\"success\">\n"
    "            <Counter/>\n"
    "        </ForceSuccess>\n"
    "    </Fallback>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui);

  if (status)
    {
      status = (1u == CounterInstruction::counter);
    }

  ASSERT_EQ(true, status);

}

TEST(Fallback, Procedure_alternative)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Fallback>\n"
    "        <Inverter name=\"failure\">\n"
    "            <Counter/>\n"
    "        </Inverter>\n"
    "        <ForceSuccess name=\"success\">\n"
    "            <Counter/>\n"
    "        </ForceSuccess>\n"
    "    </Fallback>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui);

  if (status)
    {
      status = (2u == CounterInstruction::counter);
    }

  ASSERT_EQ(true, status);

}

TEST(Fallback, Procedure_failure)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Fallback>\n"
    "        <Inverter name=\"failure\">\n"
    "            <Counter/>\n"
    "        </Inverter>\n"
    "        <Inverter name=\"failure\">\n"
    "            <Counter/>\n"
    "        </Inverter>\n"
    "    </Fallback>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  if (status)
    {
      status = (2u == CounterInstruction::counter);
    }

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
