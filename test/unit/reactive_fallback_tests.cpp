/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/compound_instruction.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::oac_tree;

const std::string ReactiveFallbackString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd">
    <ParallelSequence>
        <ReactiveFallback>
            <ReactiveSequence>
                <Equals leftVar="break" rightVar="one"/>
                <Wait name="Task" timeout="0.2"/>
            </ReactiveSequence>
            <Wait name="AlternativeTask" timeout="10.0"/>
        </ReactiveFallback>
        <Sequence>
            <Wait timeout="0.2" />
            <Copy inputVar="one" outputVar="break"/>
        </Sequence>
    </ParallelSequence>
    <Workspace>
        <Local name="break" type='{"type":"uint32"}' value="0"/>
        <Local name="zero" type='{"type":"uint32"}' value="0"/>
        <Local name="one" type='{"type":"uint32"}' value="1"/>
    </Workspace>
</Procedure>
)RAW";

TEST(ReactiveFallback, ReactiveProcedure)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(ReactiveFallbackString);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(ReactiveFallback, Setup)
{
  auto reactive_fb = GlobalInstructionRegistry().Create("ReactiveFallback");
  ASSERT_NE(reactive_fb.get(), nullptr);
  EXPECT_TRUE(reactive_fb->ChildInstructions().empty());

  auto child = GlobalInstructionRegistry().Create("Wait");
  EXPECT_TRUE(child->AddAttribute("timeout", "1.0"));
  EXPECT_TRUE(AppendChildInstruction(*reactive_fb, std::move(child)));
  EXPECT_FALSE(reactive_fb->ChildInstructions().empty());

  Procedure proc;
  EXPECT_NO_THROW(reactive_fb->Setup(proc));
}
