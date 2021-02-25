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

// Global header files

#include <gtest/gtest.h> // Google test framework

#include <common/AnyValueHelper.h>

// Local header files

#include "SequenceParser.h"
#include "Variable.h"
#include "VariableRegistry.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function declaration

static inline bool Terminate (void)
{
  bool status = false;
  if (ccs::HelperTools::Exist("/tmp/variable.bck"))
  {
    status = (std::remove("/tmp/variable.bck") == 0);
  }
  return status;
}

// Function definition

TEST(FileVariable, File_write)
{
  auto proc = sup::sequencer::ParseProcedureFile("../resources/variable_file.xml");

  bool status = bool(proc);

  if (status)
    {
      sup::sequencer::LogUI ui;
      proc->ExecuteSingle(&ui);
      status = (proc->GetStatus() == sup::sequencer::ExecutionStatus::SUCCESS);
    }

  if (status)
    {
      status = ccs::HelperTools::Exist("/tmp/variable.bck");
    }

  ccs::types::AnyValue value; // Placeholder

  if (status)
    {
      status = ccs::HelperTools::ReadFromFile(&value, "/tmp/variable.bck");
    }

  if (status)
    {
      status = static_cast<bool>(value.GetType());
    }

  Terminate();

  // ToDo - Test variable

  ASSERT_EQ(true, status);
}

TEST(FileVariable, Setup_error)
{
  auto variable = sup::sequencer::GlobalVariableRegistry().Create("FileVariable");

  bool status = static_cast<bool>(variable);

  if (status)
    { // Missing mandatory attribute .. Setup implicit
      //status = (false == variable->Setup());
      status = variable->AddAttribute("irrelevant","undefined");
    }

  ccs::types::AnyValue value; // Placeholder

  if (status)
    {
      status = ((false == variable->GetValue(value)) &&
                (false == static_cast<bool>(value.GetType())));
    }

  ASSERT_EQ(true, status);
}

TEST(FileVariable, File_error)
{
  auto variable = sup::sequencer::GlobalVariableRegistry().Create("FileVariable");

  bool status = static_cast<bool>(variable);

  if (status)
    {
      status = variable->AddAttribute("file","undefined");
    }

  ccs::types::AnyValue value; // Placeholder

  if (status)
    {
      status = ((false == variable->GetValue(value)) &&
                (false == static_cast<bool>(value.GetType())));
    }

  ASSERT_EQ(true, status);
}

TEST(FileVariable, File_attr)
{
  auto proc = sup::sequencer::ParseProcedureFile("../resources/variable_attr.xml");

  bool status = bool(proc);

  if (status)
    {
      sup::sequencer::LogUI ui;
      sup::sequencer::ExecutionStatus exec = sup::sequencer::ExecutionStatus::FAILURE;

      do
        {
          proc->ExecuteSingle(&ui);
          exec = proc->GetStatus();
        }
      while ((sup::sequencer::ExecutionStatus::SUCCESS != exec) &&
             (sup::sequencer::ExecutionStatus::FAILURE != exec));

      status = (sup::sequencer::ExecutionStatus::SUCCESS == exec);
    }

  if (status)
    {
      status = ccs::HelperTools::Exist("/tmp/variable.bck");
    }

  ccs::types::AnyValue value; // Placeholder

  if (status)
    {
      status = ccs::HelperTools::ReadFromFile(&value, "/tmp/variable.bck");
    }

  if (status)
    {
      status = static_cast<bool>(value.GetType());
    }

  // Test variable
  if (status)
    {
      status = (ccs::HelperTools::HasAttribute(&value, "severity") &&
                (ccs::types::UnsignedInteger32 == ccs::HelperTools::GetAttributeType(&value, "severity")) &&
                (7u == ccs::HelperTools::GetAttributeValue<ccs::types::uint32>(&value, "severity")));
    }

  Terminate();

  ASSERT_EQ(true, status);
}

#undef LOG_ALTERN_SRC
