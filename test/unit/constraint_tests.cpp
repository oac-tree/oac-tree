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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/sequencer/concrete_constraints.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class ConstraintTest : public ::testing::Test
{
protected:
  ConstraintTest();
  virtual ~ConstraintTest();

  ValueMap m_attr_map;
};

TEST_F(ConstraintTest, Exists)
{
  auto constraint = Constraint::Make<Exists>("empty");
  EXPECT_TRUE(constraint.Validate(m_attr_map));
}

ConstraintTest::ConstraintTest()
{
  m_attr_map["empty"] = sup::dto::AnyValue{};
}

ConstraintTest::~ConstraintTest() = default;
