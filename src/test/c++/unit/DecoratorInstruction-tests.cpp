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

#include <algorithm> // std::find

#include <gtest/gtest.h> // Google test framework

#include <common/BasicTypes.h>

#include <SequenceParser.h>

#include <Instruction.h>
#include <InstructionRegistry.h>

// Local header files

#include "UnitTestHelper.h"
#include "LogUI.h"

#include "InstructionRegistry.h"
#include "DecoratorInstruction.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();

// Function definition

// ToDo - Should implement test-specific instruction class to verify if called multiple times, or called by the ForceSuccess, etc.
//        Need for this a wait to get a reference to the instruction in the procedure.

TEST(DecoratorInstruction, ChildInstructions)
{

  sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
  bool status = (registry.RegisteredInstructionNames().end() != std::find(registry.RegisteredInstructionNames().begin(), registry.RegisteredInstructionNames().end(), "ForceSuccess"));

  std::unique_ptr<sup::sequencer::Instruction> decor;

  sup::sequencer::DecoratorInstruction* ref = NULL_PTR_CAST(sup::sequencer::DecoratorInstruction*);

  if (status)
    {
      decor = sup::sequencer::GlobalInstructionRegistry().Create("ForceSuccess");
      status = static_cast<bool>(decor);
    }

  if (status)
    {
      ref = dynamic_cast<sup::sequencer::DecoratorInstruction*>(decor.get());
      status = (NULL_PTR_CAST(sup::sequencer::DecoratorInstruction*) != ref);
    }

  if (status)
    { // HasChild is protected
      status = (true == decor->ChildInstructions().empty());
    }

  std::unique_ptr<sup::sequencer::Instruction> child;

  if (status)
    {
      child = sup::sequencer::GlobalInstructionRegistry().Create("Wait");
      status = static_cast<bool>(child);
    }

  if (status)
    {
      //ref->SetInstruction(child.get());
      ref->SetInstruction(child.release());
      status = (false == decor->ChildInstructions().empty());
    }

  ASSERT_EQ(true, status);

}

TEST(DecoratorInstruction, Inverter_success)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Inverter name=\"success\">\n"
    "            <Inverter name=\"failure\">\n"
    "                <Wait name=\"wait\" timeout=\"0.1\"/>\n"
    "            </Inverter>\n"
    "        </Inverter>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);

}

TEST(DecoratorInstruction, Inverter_failure)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Inverter name=\"failure\">\n"
    "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
    "        </Inverter>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

TEST(DecoratorInstruction, ForceSuccess_success)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <ForceSuccess name=\"success\">\n"
    "            <Wait name=\"wait\" timeout=\"0.1\"/>\n"
    "        </ForceSuccess>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);

}

TEST(DecoratorInstruction, ForceSuccess_failure)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <ForceSuccess name=\"success\">\n"
    "            <Inverter name=\"failure\">\n"
    "                <Wait name=\"wait\" timeout=\"0.1\"/>\n"
    "            </Inverter>\n"
    "        </ForceSuccess>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);

}

TEST(DecoratorInstruction, BaseClass_halt)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <ParallelSequence>\n"
    "            <ForceSuccess name=\"success\">\n"
    "                <Wait name=\"wait\" timeout=\"1.0\"/>\n"
    "            </ForceSuccess>\n"
    "            <Inverter name=\"failure\">\n"
    "                <Wait name=\"wait\" timeout=\"0.1\"/>\n"
    "            </Inverter>\n"
    "        </ParallelSequence>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
