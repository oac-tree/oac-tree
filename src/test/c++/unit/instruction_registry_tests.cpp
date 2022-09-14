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

#include <sup/sequencer/instruction_registry.h>

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/sequence_parser.h>

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

TEST(InstructionRegistry, RegisterInstruction)
{
  // no such name
  auto registered_names = sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
  auto it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it == registered_names.end());

  // not possible to create instruction
  auto inst = sup::sequencer::GlobalInstructionRegistry().Create(TestInstruction::Type);
  EXPECT_FALSE(inst.get());

  // registration
  EXPECT_TRUE(sup::sequencer::RegisterGlobalInstruction<TestInstruction>());

  // second registration with the same name is not allowed
  EXPECT_THROW(sup::sequencer::RegisterGlobalInstruction<TestInstruction>(), std::runtime_error);

  // there is a name
  registered_names = sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
  it = std::find(registered_names.begin(), registered_names.end(), TestInstruction::Type);
  EXPECT_TRUE(it != registered_names.end());

  // can create instruction
  inst = sup::sequencer::GlobalInstructionRegistry().Create(TestInstruction::Type);
  EXPECT_TRUE(inst.get());
}

TEST(InstructionRegistry, CreateFailure)
{
  auto inst = sup::sequencer::GlobalInstructionRegistry().Create("UndefinedInstructionName");
  EXPECT_FALSE(inst.get());
}
