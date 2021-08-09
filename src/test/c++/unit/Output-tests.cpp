/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Gennady Pospelov (IO)
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

#include <gtest/gtest.h>

#include <common/BasicTypes.h>

#include <SequenceParser.h>

// Local header files

#include "UnitTestHelper.h"
#include "UserInterface.h"

//! Testing Output instruction.
class OutputTest : public ::testing::Test {
public:

  //! Test interface which accepts values from the workspace.
  class TestInterface : public ::sup::sequencer::UserInterface {
  public:
    TestInterface(unsigned par) : m_value(ccs::types::UnsignedInteger32){
      m_value = static_cast<ccs::types::uint32>(par);
    }

    bool PutValueImpl(const ::ccs::types::AnyValue &value,
                      const std::string &description) override {
      m_value = value;
      return true;
    }

    bool GetUserValueImpl (::ccs::types::AnyValue &, const std::string&) override {
      return true;
    }

    void UpdateInstructionStatusImpl(
        const sup::sequencer::Instruction *instruction) override {}
    ::ccs::types::AnyValue m_value;

  };

};

// Takes integer value from the workspace and put in into TestInterface.

TEST_F(OutputTest, PutInteger) {
  static const std::string procedure_body { 
  R"RAW(
  <Sequence>
    <Output from="var1"/>
  </Sequence>
  <Workspace>
    <Local name="var1" type='{"type":"uint32"}' value='42' />
  </Workspace>
)RAW"};

  const auto procedure_string = ::sup::UnitTestHelper::CreateProcedureString(procedure_body);

  const unsigned expected{22};
  TestInterface ui{expected};
  // initial state
  EXPECT_EQ(static_cast<ccs::types::uint32>(ui.m_value), static_cast<ccs::types::uint32>(expected));

  auto proc = sup::sequencer::ParseProcedureString(procedure_string);
  ASSERT_TRUE(proc.get() != nullptr);
  ASSERT_TRUE(sup::UnitTestHelper::TryAndExecute(
      proc, &ui, sup::sequencer::ExecutionStatus::SUCCESS));

  // checking that Output instruction has propagated the value to the interface
  EXPECT_EQ(static_cast<ccs::types::uint32>(ui.m_value), static_cast<ccs::types::uint32>(42));
}
