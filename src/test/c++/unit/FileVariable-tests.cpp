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

// Local header files

#include "SequenceParser.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "unit-test"

// Type declaration

// Function definition

TEST(FileVariable, Write)
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

  ccs::types::value value; // Placeholder

  if (status)
    {
      status = ccs::HelperTools::ReadFromFile(&value, "/tmp/variable.bck");
    }

  if (status)
    {
      status = static_cast<bool>(value.GetType());
    }

  // ToDo - Test variable

  ASSERT_EQ(true, status);
}

#undef LOG_ALTERN_SRC
