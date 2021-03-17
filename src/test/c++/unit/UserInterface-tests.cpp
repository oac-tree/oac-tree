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
#include <algorithm>

// Local header files

#include "MockUserInterface.h"
#include "Instruction.h"
#include "InstructionRegistry.h"
#include "Workspace.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition
using namespace sup::sequencer;

class UserInterfaceTest : public ::testing::Test {
  protected:
    UserInterfaceTest();
    virtual ~UserInterfaceTest();

    MockUserInterface mock_ui;
    Workspace empty_ws;
};

// Function declaration

// Global variables

static ::ccs::log::Func_t __handler = ::ccs::log::SetStdout();

static const std::string TREE_TYPE = "TestData";

// Function definition

using ::testing::InSequence;
using ::testing::_;

MATCHER_P(HasExecutionStatus, expected, "") { return arg->GetStatus() == expected; }

TEST_F(UserInterfaceTest, Constructed)
{
  auto wait = GlobalInstructionRegistry().Create("Wait");
  EXPECT_THAT(wait, HasExecutionStatus(ExecutionStatus::NOT_STARTED));
  {
    InSequence seq;
    EXPECT_CALL(mock_ui, UpdateInstructionStatusImpl(HasExecutionStatus(ExecutionStatus::NOT_FINISHED)));
    EXPECT_CALL(mock_ui, UpdateInstructionStatusImpl(HasExecutionStatus(ExecutionStatus::SUCCESS)));
  }
  wait->ExecuteSingle(&mock_ui, &empty_ws);
}

UserInterfaceTest::UserInterfaceTest()
  : mock_ui{}
  , empty_ws{}
{
}

UserInterfaceTest::~UserInterfaceTest() = default;

#undef LOG_ALTERN_SRC
