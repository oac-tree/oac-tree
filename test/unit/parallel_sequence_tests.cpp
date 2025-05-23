/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
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

const std::string ProcedureSequenceString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd">
    <Sequence>
        <Wait name="wait"/>
        <Wait name="again"/>
    </Sequence>
    <Workspace>
    </Workspace>
</Procedure>
)RAW";

const std::string ProcedureParallelString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd">
    <ParallelSequence name="parallel" successThreshold="1" failureThreshold="2">
        <Wait name="wait"/>
        <Wait name="again" timeout="0.1"/>
    </ParallelSequence>
    <Workspace>
    </Workspace>
</Procedure>
)RAW";

const std::string ProcedureParallelBuiltinString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/oac-tree" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/oac-tree oac-tree.xsd">
    <ParallelSequence name="parallel" successThreshold="1" failureThreshold="3">
        <Wait name="wait" timeout="2.0"/>
        <Copy name="copy" inputVar="input" outputVar="output"/>
        <Wait name="again" timeout="2.0"/>
    </ParallelSequence>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
</Procedure>
)RAW";

TEST(ParallelSequence, Procedure_sequence)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(ProcedureSequenceString);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(ParallelSequence, Procedure_parallel)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(ProcedureParallelString);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(ParallelSequence, WithBuiltinCode)
{
  sup::UnitTestHelper::EmptyUserInterface ui;
  auto proc = ParseProcedureString(ProcedureParallelBuiltinString);
  EXPECT_TRUE(sup::UnitTestHelper::TryAndExecute(proc, ui));
}

TEST(ParallelSequence, SetupImpl_thresholds)
{
  auto parallel = GlobalInstructionRegistry().Create("ParallelSequence");
  ASSERT_NE(parallel.get(), nullptr);
  EXPECT_TRUE(parallel->ChildInstructions().empty());
  EXPECT_TRUE(parallel->AddAttribute("successThreshold", "undefined")
              && parallel->AddAttribute("failureThreshold", "undefined"));

  auto child = GlobalInstructionRegistry().Create("Wait");
  EXPECT_TRUE(child->AddAttribute("timeout", "1.0"));
  EXPECT_TRUE(AppendChildInstruction(*parallel, std::move(child)));
  EXPECT_FALSE(parallel->ChildInstructions().empty());

  Procedure proc;
  EXPECT_THROW(parallel->Setup(proc), InstructionSetupException);
}
