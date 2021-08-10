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

#include "SequenceParser.h"
#include "UnitTestHelper.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

using namespace sup::sequencer;

class ParserTest : public ::testing::Test
{
protected:
  ParserTest();
  virtual ~ParserTest();

  std::unique_ptr<Procedure> CreateProcedure(const std::string& body)
  {
    return ParseProcedureString(::sup::UnitTestHelper::CreateProcedureString(body));
  }

  std::string incorrect_root_file;
  std::string syntax_error_file;
};

const std::string ProcedureOnlyString{R"(
    <Sequence name="Main Sequence">
        <Wait name="Immediate Success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)"};

const std::string WorkspaceOnlyString{R"(
    <Workspace>
        <Local name="a" type='{"type":"int8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='0' />
    </Workspace>
)"};

const std::string IncorrectRootStringProcedure =
    R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Sequence xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
          name="Procedure containing the wrong root element type for testing purposes"
          xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
          xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Wait name="Immediate Success"/>
    <Wait name="One" timeout="1.0"/>
</Sequence>  
)RAW";

static const std::string XMLSyntaxErrorStringProcedure =
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

const std::string NonExistentInstructionString{R"(
    <Sequence name="Main Sequence">
        <DoesNotExist name="Cannot be parsed"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)"};

const std::string NonExistentVariableString{R"(
    <Wait name="Immediate success" />
    <Workspace>
        <DoesNotExist name="Cannot be parsed" />
    </Workspace>
)"};

const std::string NonExistentPluginString{R"(
    <Plugin>libNonExistent.so</Plugin>
    <Sequence name="Main Sequence">
        <Wait name="Immediate success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
)"};

const std::string InvalidChildString{R"(
    <Wait name="Immediate Success">
        <Wait name="One" timeout="1.0"/>
    </Wait>
)"};

TEST_F(ParserTest, Successful)
{
  auto proc = CreateProcedure(ProcedureOnlyString);
  EXPECT_TRUE(static_cast<bool>(proc));
}

TEST_F(ParserTest, WorkspaceOnly)
{
  auto proc = CreateProcedure(WorkspaceOnlyString);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_EQ(proc->RootInstrunction(), nullptr);
}

TEST_F(ParserTest, IncorrectRoot)
{
  auto proc = ParseProcedureString(IncorrectRootStringProcedure);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, XMLSyntaxError)
{
  auto proc = ParseProcedureString(XMLSyntaxErrorStringProcedure);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, NonExistentInstructionError)
{
  auto proc = CreateProcedure(NonExistentInstructionString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, NonExistentVariableError)
{
  auto proc = CreateProcedure(NonExistentVariableString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, NonExistentPluginError)
{
  auto proc = CreateProcedure(NonExistentPluginString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, InvalidChildError)
{
  auto proc = CreateProcedure(InvalidChildString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, IncorrectRootFromFile)
{
  auto proc = ParseProcedureFile(incorrect_root_file);
  EXPECT_FALSE(static_cast<bool>(proc));
}

TEST_F(ParserTest, XMLSyntaxErrorFromFile)
{
  auto proc = ParseProcedureFile(syntax_error_file);
  EXPECT_FALSE(static_cast<bool>(proc));
}

ParserTest::ParserTest()
    : incorrect_root_file("/tmp/incorrect_root.xml"), syntax_error_file("/tmp/syntax_error.xml")
{
  {
    std::ofstream of_str(incorrect_root_file);
    of_str << IncorrectRootStringProcedure;
  }
  {
    std::ofstream of_str(syntax_error_file);
    of_str << XMLSyntaxErrorStringProcedure;
  }
}

ParserTest::~ParserTest()
{
  std::remove(incorrect_root_file.c_str());
  std::remove(syntax_error_file.c_str());
}
