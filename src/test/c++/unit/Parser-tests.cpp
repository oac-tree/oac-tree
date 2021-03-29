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

// Global header files

#include <gtest/gtest.h> // Google test framework
#include <common/log-api.h> // Syslog wrapper routines
#include <fstream> // std::ofstream
#include <cstdio> // std::remove

// Local header files

#include "SequenceParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

using namespace sup::sequencer;

class ParserTest : public ::testing::Test
{
  protected:
    ParserTest();
    virtual ~ParserTest();

    std::string incorrect_root_file;
    std::string syntax_error_file;
};

// Function declaration

// Global variables

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

static const std::string ProcedureOnlyString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing no workspace for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Sequence name="Main Sequence">
        <Wait name="Immediate Success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
</Procedure>
)RAW";

static const std::string WorkspaceOnlyString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing only a workspace for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Workspace>
        <Local name="a" type='{"type":"int8"}' value='1' />
        <Local name="b" type='{"type":"uint8"}' value='0' />
    </Workspace>
</Procedure>
)RAW";

static const std::string IncorrectRootString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Sequence xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
          name="Procedure containing the wrong root element type for testing purposes"
          xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
          xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Wait name="Immediate Success"/>
    <Wait name="One" timeout="1.0"/>
</Sequence>
)RAW";

static const std::string XMLSyntaxErrorString =
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

static const std::string NonExistentInstructionString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing non-existent instruciton for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Sequence name="Main Sequence">
        <DoesNotExist name="Cannot be parsed"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
</Procedure>
)RAW";

static const std::string NonExistentVariableString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing only a workspace for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Wait name="Immediate success" />
    <Workspace>
        <DoesNotExist name="Cannot be parsed" />
    </Workspace>
</Procedure>
)RAW";

static const std::string NonExistentPluginString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing non-existent instruciton for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Plugin>libNonExistent.so</Plugin>
    <Sequence name="Main Sequence">
        <Wait name="Immediate success"/>
        <Wait name="One" timeout="1.0"/>
    </Sequence>
</Procedure>
)RAW";

static const std::string InvalidChildString =
R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Procedure containing invalid child of leaf instruction for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">
    <Wait name="Immediate Success">
        <Wait name="One" timeout="1.0"/>
    </Wait>
</Procedure>
)RAW";

// Function definition

TEST(Parser, Successful)
{
  auto proc = ParseProcedureString(ProcedureOnlyString);
  EXPECT_TRUE(static_cast<bool>(proc));
}

TEST(Parser, WorkspaceOnly)
{
  auto proc = ParseProcedureString(WorkspaceOnlyString);
  ASSERT_TRUE(static_cast<bool>(proc));
  EXPECT_EQ(proc->RootInstrunction(), nullptr);
}

TEST(Parser, IncorrectRoot)
{
  auto proc = ParseProcedureString(IncorrectRootString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST(Parser, XMLSyntaxError)
{
  auto proc = ParseProcedureString(XMLSyntaxErrorString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST(Parser, NonExistentInstructionError)
{
  auto proc = ParseProcedureString(NonExistentInstructionString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST(Parser, NonExistentVariableError)
{
  auto proc = ParseProcedureString(NonExistentVariableString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST(Parser, NonExistentPluginError)
{
  auto proc = ParseProcedureString(NonExistentPluginString);
  ASSERT_FALSE(static_cast<bool>(proc));
}

TEST(Parser, InvalidChildError)
{
  auto proc = ParseProcedureString(InvalidChildString);
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
  : incorrect_root_file("/tmp/incorrect_root.xml")
  , syntax_error_file("/tmp/syntax_error.xml")
{
  {
    std::ofstream of_str(incorrect_root_file);
    of_str << IncorrectRootString;
  }
  {
    std::ofstream of_str(syntax_error_file);
    of_str << XMLSyntaxErrorString;
  }
}

ParserTest::~ParserTest()
{
  std::remove(incorrect_root_file.c_str());
  std::remove(syntax_error_file.c_str());
}

#undef LOG_ALTERN_SRC
