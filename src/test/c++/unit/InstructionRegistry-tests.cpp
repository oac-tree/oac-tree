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

#include <common/log-api.h>

// Local header files

#include "InstructionRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

class TestInstruction : public sup::sequencer::Instruction
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

    TestInstruction (void);

    /**
     * @brief Destructor.
     */

    ~TestInstruction (void) override;

    /**
     * @brief Class name for InstructionRegistry.
     */

    static const std::string Type;

};

// Function declaration

// Global variables

static ccs::log::Func_t _log_handler = ccs::log::SetStdout();
const std::string TestInstruction::Type = "TestInstruction";

// Function definition

sup::sequencer::ExecutionStatus TestInstruction::ExecuteSingleImpl (sup::sequencer::UserInterface * ui, sup::sequencer::Workspace * ws) {}
TestInstruction::TestInstruction() : sup::sequencer::Instruction(TestInstruction::Type) {}
TestInstruction::~TestInstruction() {}

TEST(InstructionRegistry, Register_success)
{

  bool status = sup::sequencer::RegisterGlobalInstruction<TestInstruction>();

  if (status)
    {
      sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
      status = (registry.RegisteredInstructionNames().end() != std::find(registry.RegisteredInstructionNames().begin(), registry.RegisteredInstructionNames().end(), TestInstruction::Type));
    }

  ASSERT_EQ(true, status);

}

TEST(InstructionRegistry, Create_success)
{

  sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
  bool status = (registry.RegisteredInstructionNames().end() != std::find(registry.RegisteredInstructionNames().begin(), registry.RegisteredInstructionNames().end(), TestInstruction::Type));

  if (!status)
    {
      status = sup::sequencer::RegisterGlobalInstruction<TestInstruction>();
    } 

  if (status)
    {
      auto inst = sup::sequencer::GlobalInstructionRegistry().Create(TestInstruction::Type);
      status = static_cast<bool>(inst);
    }

  ASSERT_EQ(true, status);

}

TEST(InstructionRegistry, Create_failure)
{

  sup::sequencer::InstructionRegistry registry = sup::sequencer::GlobalInstructionRegistry();
  auto inst = sup::sequencer::GlobalInstructionRegistry().Create("UndefinedInstructionName");
  bool status = (false == static_cast<bool>(inst));

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
