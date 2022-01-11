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

#include "InstructionRegistry.h"

#include <Instruction.h>
#include <InstructionRegistry.h>
#include <SequenceParser.h>
#include <common/BasicTypes.h>
#include <gtest/gtest.h>

#include <algorithm>

class TestInstruction : public sup::sequencer::Instruction
{
public:
  TestInstruction() : sup::sequencer::Instruction(TestInstruction::Type) {}
  static const std::string Type;
  
private:
  sup::sequencer::ExecutionStatus ExecuteSingleImpl(sup::sequencer::UserInterface* ui,
                                                    sup::sequencer::Workspace* ws) override
  {
    return sup::sequencer::ExecutionStatus::SUCCESS;
  }
};

const std::string TestInstruction::Type = "TestInstruction";

TEST(InstructionRegistry, Register_success)
{
  EXPECT_TRUE(sup::sequencer::RegisterGlobalInstruction<TestInstruction>());

  auto registered_names = sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
  auto it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it != registered_names.end());
}

TEST(InstructionRegistry, Create_success)
{
  auto registered_names = sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
  auto it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it == registered_names.end());

  EXPECT_TRUE(sup::sequencer::RegisterGlobalInstruction<TestInstruction>());
  auto inst = sup::sequencer::GlobalInstructionRegistry().Create(TestInstruction::Type);
  EXPECT_TRUE(inst.get());
}

TEST(InstructionRegistry, Create_failure)
{
  auto inst = sup::sequencer::GlobalInstructionRegistry().Create("UndefinedInstructionName");
  EXPECT_FALSE(inst.get());
}
