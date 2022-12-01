/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP Sequencer
 *
 * Description   : Unit test code
 *
 * Author        : Walter Van Herck (IO)
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

#include <sup/sequencer/sequence_parser.h>

#include "unit_test_helper.h"

#include <sup/sequencer/instructions/sequence.h>

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class SequencerParserTest : public ::testing::Test
{
protected:
  //! Returns multi-line XML string with Sequencer procedure, where user body
  //! is wrapped into necessary elements. Procedure tag deliberately doesn't contain any schema.
  std::string CreateProcedureString(const std::string& body)
  {
    static const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>)RAW"};
    static const std::string footer{R"RAW(</Procedure>
)RAW"};
    return header + body + footer;
  }

  std::unique_ptr<Procedure> CreateProcedure(const std::string& body)
  {
    return ParseProcedureString(
        sup::UnitTestHelper::CreateProcedureString(body));
  }
};

TEST_F(SequencerParserTest, ProcedureOnlyString)
{
  const std::string body{R"(
    <Sequence name="Main Sequence">
        <Wait name="Immediate Success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)"};

  auto proc = CreateProcedure(body);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_EQ(proc->RootInstruction()->GetType(), Sequence::Type);
  EXPECT_EQ(proc->RootInstruction()->ChildrenCount(), 2);
  ASSERT_NE(proc->GetWorkspace(), nullptr);
  ASSERT_EQ(proc->GetWorkspace()->GetVariables().size(), 0);
}

TEST_F(SequencerParserTest, WorkspaceOnly)
{
  const std::string body{R"(
    <Workspace>
        <Local name="a" type='{"type":"int8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='0' />
    </Workspace>
)"};

  auto proc = CreateProcedure(body);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_EQ(proc->RootInstruction(), nullptr);
}

TEST_F(SequencerParserTest, IncorrectRoot)
{
  const std::string body =
      R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Sequence xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
          name="Procedure containing the wrong root element type for testing purposes"
          xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
          xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Wait name="Immediate Success"/>
    <Wait name="One" timeout="1.0"/>
</Sequence>
)RAW";

  EXPECT_THROW(ParseProcedureString(body), ParseException);

  // same via file on disk
  const std::string file_name("incorrect_root.xml");
  sup::UnitTestHelper::TemporaryTestFile test_file(file_name, body);
  EXPECT_THROW(ParseProcedureFile(file_name), ParseException);
}

TEST_F(SequencerParserTest, XMLSyntaxError)
{
  static const std::string body =
      R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing a XML syntax error for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Sequence name="Main Sequence">
        <Wait name="Immediate Success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)RAW";

  auto proc = ParseProcedureString(body);
  ASSERT_FALSE(static_cast<bool>(proc));

  const std::string file_name("syntax_error.xml");
  sup::UnitTestHelper::TemporaryTestFile test_file(file_name, body);
  proc = ParseProcedureFile(file_name);
  EXPECT_FALSE(static_cast<bool>(proc));
}

TEST_F(SequencerParserTest, NonExistentInstructionError)
{
  const std::string body{R"(
    <Sequence name="Main Sequence">
        <DoesNotExist name="Cannot be parsed"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)"};
  EXPECT_THROW(CreateProcedure(body), ParseException);
}

TEST_F(SequencerParserTest, NonExistentVariableError)
{
  const std::string body{R"(
    <Wait name="Immediate success" />
    <Workspace>
        <DoesNotExist name="Cannot be parsed" />
    </Workspace>
)"};

  EXPECT_THROW(CreateProcedure(body), ParseException);
}

TEST_F(SequencerParserTest, NonExistentPluginError)
{
  const std::string body{R"(
    <Plugin>libNonExistent.so</Plugin>
    <Sequence name="Main Sequence">
        <Wait name="Immediate success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)"};

  EXPECT_THROW(CreateProcedure(body), RuntimeException);
}

TEST_F(SequencerParserTest, InvalidChildError)
{
  const std::string body{R"(
    <Wait name="Immediate Success">
        <Wait name="One" timeout="1.0"/>
    </Wait>
)"};
  EXPECT_THROW(CreateProcedure(body), ParseException);
}

TEST_F(SequencerParserTest, SequenceWithInverter)
{
  const std::string body{R"(
    <Sequence>
        <Wait name="First" />
        <Wait name="Second" />
        <Inverter>
            <Wait name="Third" />
        </Inverter>
    </Sequence>
    <Workspace>
    </Workspace>
)"};

  const std::string file_name = "sequence_with_inverter.xml";
  sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = ParseProcedureFile(file_name);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_EQ(proc->GetInstructionCount(), 1);
  EXPECT_EQ(proc->RootInstruction()->ChildrenCount(), 3);
}

TEST_F(SequencerParserTest, Workspace)
{
  const std::string body{R"(
    <Sequence>
        <Wait name="Only" />
    </Sequence>
    <Workspace>
        <Local name="var1"
               type='{"type":"vartype1","attributes":[{"value":{"type":"uint64"}}]}'
               value='{"value":1729}' />
        <Local name="var2"
               type='{"type":"vartype2","attributes":[{"value":{"type":"string"}}]}' />
        <Local name="var3"
               type='{"type":"vartype3","attributes":[{"value":{"type":"bool"}}]}'/>
    </Workspace>
)"};

  const std::string file_name = "workspace.xml";
  sup::UnitTestHelper::TemporaryTestFile test_file(
      file_name, sup::UnitTestHelper::CreateProcedureString(body));

  auto proc = ParseProcedureFile(file_name);
  ASSERT_TRUE(static_cast<bool>(proc));
  auto variables = proc->GetWorkspace()->GetVariables();
  ASSERT_EQ(variables.size(), 3);
  EXPECT_EQ(variables.at(0)->GetAttribute("name"), "var1");
  EXPECT_EQ(variables.at(1)->GetAttribute("name"), "var2");
  EXPECT_EQ(variables.at(2)->GetAttribute("name"), "var3");
}

TEST_F(SequencerParserTest, EmptyProcedureFromXMLAndBack)
{
  const std::string body{R"(
  <Workspace/>
)"};

  auto xml_string = CreateProcedureString(body);
  auto procedure = ParseProcedureString(xml_string);
  ASSERT_TRUE(static_cast<bool>(procedure));
  EXPECT_EQ(xml_string, GetXMLString(*procedure));
}

TEST_F(SequencerParserTest, ProcedureWithSequenceAndVariableFromXMLAndBack)
{
  const std::string body{R"(
  <Sequence>
    <Wait name="wait" timeout="0.1"/>
  </Sequence>
  <Workspace>
    <Local name="input"/>
  </Workspace>
)"};

  auto xml_string = CreateProcedureString(body);
  auto procedure = ParseProcedureString(xml_string);
  ASSERT_TRUE(static_cast<bool>(procedure));
  EXPECT_EQ(xml_string, GetXMLString(*procedure));
}
