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

#include "parser/SequenceParser.h"

// Constants

// Function declaration

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

  log_info("Parse simpletest.xml..");
  auto proc = SequenceParser::ParseProcedureXML("../../simpletest.xml");

  status = bool(proc);

  Terminate();

  ASSERT_EQ(true, status);
}

