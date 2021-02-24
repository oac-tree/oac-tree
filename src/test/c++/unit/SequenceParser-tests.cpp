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

// Local header files

#include "InstructionRegistry.h"
#include "SequenceParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Function declaration

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure * procedure);

// Global variables

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

// Function definition

static inline bool Initialise (void)
{
  return true;
}

static inline bool Terminate (void)
{
  return true;
}

TEST(SequenceParser, Default) // Static initialisation
{
  bool status = Initialise();

  auto proc = sup::sequencer::ParseProcedureFile("../resources/sequence_fail.xml");

  status = bool(proc);

  Terminate();

  ASSERT_EQ(true, status);
}

TEST(SequenceParser, Workspace) // Static initialisation
{
  bool status = Initialise();

  auto proc = sup::sequencer::ParseProcedureFile("../resources/workspace.xml");

  status = bool(proc);

  if (status)
  {
    status = PrintProcedureWorkspace(proc.get());
  }

  Terminate();

  ASSERT_EQ(true, status);
}

TEST(SequenceParser, ParseString) // Static initialisation
{
  bool status = Initialise();

  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Sequence>\n"
    "        <Wait name=\"First\" />\n"
    "        <Wait name=\"Second\" />\n"
    "    </Sequence>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  status = bool(proc);

  if (status)
  {
    status = PrintProcedureWorkspace(proc.get());
  }

  Terminate();

  ASSERT_EQ(true, status);
}


static bool PrintProcedureWorkspace(::sup::sequencer::Procedure * procedure)
{
  auto var_names = procedure->VariableNames();
  ::ccs::types::char8 val_string[1024];
  for (const auto & var_name : var_names)
  {
    ::ccs::types::AnyValue val;
    bool var_initialized = procedure->GetVariableValue(var_name, val);
    if (var_initialized)
    {
      val.SerialiseInstance(val_string, 1024);
      log_info("Variable '%s', with value\n  %s", var_name.c_str(), val_string);
    }
    else
    {
      log_info("Variable '%s' uninitialized", var_name.c_str());
    }
  }
  return true;
}

#undef LOG_ALTERN_SRC
