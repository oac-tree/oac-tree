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

#include "CompoundInstruction.h"
#include "Instruction.h"
#include "InstructionRegistry.h"
#include "LogUI.h"
#include "UnitTestHelper.h"
#include "Workspace.h"

#include <SequenceParser.h>
#include <gtest/gtest.h>

#include <algorithm>

using namespace sup::sequencer;

class CopyInstruction : public Instruction
{
public:
  CopyInstruction() : sup::sequencer::Instruction(CopyInstruction::Type) {}
  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
  {
    bool status = (HasAttribute("input") && HasAttribute("output"));
    ccs::types::AnyValue _value;
    if (status)
    {  // Read from workspace
      status = ws->GetValue(GetAttribute("input"), _value);
    }
    if (status)
    {  // Write to workspace
      status = ws->SetValue(GetAttribute("output"), _value);
    }
    return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);
  }
};

const std::string CopyInstruction::Type = "CopyInstruction";

static const std::string ProcedureSequenceString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Repeat maxCount="3">
        <Sequence>
            <Wait name="wait" timeout="0.1"/>
            <Wait name="again" timeout="0.01"/>
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
    <Repeat maxCount="3">
        <ParallelSequence name="parallel" successThreshold="1" failureThreshold="2">
            <Wait name="wait" timeout="0.01"/>
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
    <Repeat maxCount="3">
        <ParallelSequence name="parallel" successThreshold="1" failureThreshold="3">
            <Wait name="wait" timeout="0.01"/>
            <Copy name="copy" input="input" output="output"/>
            <Wait name="again" timeout="0.1"/>
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
    <Repeat maxCount="3">
        <ParallelSequence name="parallel" successThreshold="1" failureThreshold="3">
            <Wait name="wait" timeout="0.01"/>
            <CopyInstruction name="copy" input="input" output="output"/>
            <Wait name="again" timeout="0.1"/>
        </ParallelSequence>
    </Repeat>
    <Workspace>
        <Local name="input" type='{"type":"string"}' value='"undefined"'/>
        <Local name="output" type='{"type":"string"}' value='"denifednu"'/>
    </Workspace>
</Procedure>
)RAW";

static const std::string ProcedureThresholdsString =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Repeat maxCount="3">
        <ParallelSequence name="parallel" successThreshold="undefined" failureThreshold="undefined">
            <Wait name="wait" timeout="0.1"/>
        </ParallelSequence>
    </Repeat>
    <Workspace>
    </Workspace>
</Procedure>
)RAW";

TEST(ParallelSequence, Procedure_sequence)
{
  bool status = true;
  try
  {
    sup::sequencer::LogUI ui;
    auto proc = sup::sequencer::ParseProcedureString(ProcedureSequenceString);
    status = sup::UnitTestHelper::TryAndExecute(proc, &ui);
  }
  catch (...)
  {
    status = false;
  }
  ASSERT_EQ(true, status);
}

TEST(ParallelSequence, Procedure_parallel)
{
  bool status = true;
  try
  {
    sup::sequencer::LogUI ui;
    auto proc = sup::sequencer::ParseProcedureString(ProcedureParallelString);

    status = sup::UnitTestHelper::TryAndExecute(proc, &ui);
  }
  catch (...)
  {
    status = false;
  }
  ASSERT_EQ(true, status);
}

TEST(ParallelSequence, WithBuiltinCode)
{
  bool status = true;
  try
  {
    sup::sequencer::LogUI ui;
    auto proc = sup::sequencer::ParseProcedureString(ProcedureParallelBuiltinString);

    status = sup::UnitTestHelper::TryAndExecute(proc, &ui);
  }
  catch (...)
  {
    status = false;
  }
  ASSERT_EQ(true, status);
}

TEST(ParallelSequence, WithUserCode)
{
  bool status = sup::sequencer::RegisterGlobalInstruction<CopyInstruction>();
  try
  {
    sup::sequencer::LogUI ui;
    auto proc = sup::sequencer::ParseProcedureString(ProcedureParallelUserCodeString);

    status = sup::UnitTestHelper::TryAndExecute(proc, &ui);
  }
  catch (...)
  {
    status = false;
  }
  ASSERT_EQ(true, status);
}

TEST(ParallelSequence, SetupImpl_thresholds)
{
  auto names = sup::sequencer::GlobalInstructionRegistry().RegisteredInstructionNames();
  ASSERT_TRUE(std::find(names.begin(), names.end(), "ParallelSequence") != names.end());

  auto parallel = sup::sequencer::GlobalInstructionRegistry().Create("ParallelSequence");
  auto compound = dynamic_cast<sup::sequencer::CompoundInstruction *>(parallel.get());
  EXPECT_TRUE(compound);

  EXPECT_TRUE(parallel->ChildInstructions().empty());
  EXPECT_TRUE(parallel->AddAttribute("successThreshold", "undefined")
              && parallel->AddAttribute("failureThreshold", "undefined"));

  auto child = sup::sequencer::GlobalInstructionRegistry().Create("Wait");
  EXPECT_TRUE(child->AddAttribute("timeout", "1.0"));

  compound->PushBack(child.release());
  EXPECT_FALSE(parallel->ChildInstructions().empty());

  sup::sequencer::Procedure proc;
  EXPECT_FALSE(parallel->Setup(proc));
}
