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

#include <gtest/gtest.h>
#include "Instruction.h"
#include "Procedure.h"
#include "Variable.h"

using namespace sup::sequencer;

class ProcedureToTreeDataUtilsTest : public ::testing::Test
{
};

TEST_F(ProcedureToTreeDataUtilsTest, VariableConversion)
{
  EXPECT_EQ(1,1);
}
