/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP oac-tree
 *
 * Description   : Unit test code
 *
 * Author        : B.Bauvir (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/workspace.h>

#include <gtest/gtest.h>

const std::string USER_CONFIRMATION_TYPE = "UserConfirmation";

const std::string DESCRIPTION_ATTRIBUTE = "description";
const std::string OK_ATTRIBUTE = "okText";
const std::string CANCEL_ATTRIBUTE = "cancelText";

const std::string DEFAULT_OK_TEXT = "Ok";
const std::string DEFAULT_CANCEL_TEXT = "Cancel";

using namespace sup::oac_tree;
using sup::UnitTestHelper::TryAndExecute;

TEST(UserConfirmation, GetUserConfirmation)
{
  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
    <UserConfirmation description="Ok to proceed?"/>
    <Workspace/>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  // zero indicates success branch
  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui));
  auto options = ui.GetOptions();
  ASSERT_EQ(options.size(), 2);
  EXPECT_EQ(options[0], DEFAULT_OK_TEXT);
  EXPECT_EQ(options[1], DEFAULT_CANCEL_TEXT);
  auto metadata = ui.GetMetadata();
  ASSERT_TRUE(metadata->HasField(Constants::USER_CHOICES_TEXT_NAME));

  // every other return value indicates failure branch
  ui.SetChoice(1);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));

  // Invalid choices
  ui.SetChoice(2);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));

  ui.SetChoice(-1);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(UserConfirmation, Attributes)
{
  Procedure proc;
  auto instr = GlobalInstructionRegistry().Create(USER_CONFIRMATION_TYPE);
  ASSERT_NE(instr.get(), nullptr);

  // Missing mandatory attribute
  EXPECT_THROW(instr->Setup(proc), InstructionSetupException);

  // Add mandatory attribute
  const std::string main_text = "does_not_matter";
  instr->AddAttribute(DESCRIPTION_ATTRIBUTE, main_text);
  EXPECT_NO_THROW(instr->Setup(proc));
}

TEST(UserConfirmation, OptionLabels)
{
  Procedure proc;
  Workspace ws;
  sup::UnitTestHelper::MockUI ui;

  {
    // Running UserConfirmation with default options
    auto instr = GlobalInstructionRegistry().Create(USER_CONFIRMATION_TYPE);
    ASSERT_NE(instr.get(), nullptr);
    const std::string main_text = "does_not_matter";
    instr->AddAttribute(DESCRIPTION_ATTRIBUTE, main_text);
    EXPECT_NO_THROW(instr->Setup(proc));

    ui.SetChoice(0);
    EXPECT_NO_THROW(instr->ExecuteSingle(ui, ws));
    EXPECT_EQ(instr->GetStatus(), ExecutionStatus::SUCCESS);

    // Check passed default option values
    auto options = ui.GetOptions();
    ASSERT_EQ(options.size(), 2);
    EXPECT_EQ(options[0], DEFAULT_OK_TEXT);
    EXPECT_EQ(options[1], DEFAULT_CANCEL_TEXT);
  }
  {
    // Running UserConfirmation with custom option labels
    auto instr = GlobalInstructionRegistry().Create(USER_CONFIRMATION_TYPE);
    ASSERT_NE(instr.get(), nullptr);
    const std::string main_text = "Proceed?";
    const std::string ok_label = "yes";
    const std::string cancel_label = "no";
    instr->AddAttribute(DESCRIPTION_ATTRIBUTE, main_text);
    instr->AddAttribute(OK_ATTRIBUTE, ok_label);
    instr->AddAttribute(CANCEL_ATTRIBUTE, cancel_label);
    EXPECT_NO_THROW(instr->Setup(proc));

    ui.SetChoice(1);
    EXPECT_NO_THROW(instr->ExecuteSingle(ui, ws));
    EXPECT_EQ(instr->GetStatus(), ExecutionStatus::FAILURE);

    // Check passed default option values
    auto options = ui.GetOptions();
    ASSERT_EQ(options.size(), 2);
    EXPECT_EQ(options[0], ok_label);
    EXPECT_EQ(options[1], cancel_label);
  }
}

TEST(UserConfirmation, Metadata)
{
  Procedure proc;
  Workspace ws;
  sup::UnitTestHelper::MockUI ui;

  auto instr = GlobalInstructionRegistry().Create(USER_CONFIRMATION_TYPE);
  ASSERT_NE(instr.get(), nullptr);
  const std::string main_text = "does_not_matter";
  instr->AddAttribute(DESCRIPTION_ATTRIBUTE, main_text);
  EXPECT_NO_THROW(instr->Setup(proc));

  ui.SetChoice(0);
  EXPECT_NO_THROW(instr->ExecuteSingle(ui, ws));
  EXPECT_EQ(instr->GetStatus(), ExecutionStatus::SUCCESS);

  // Check passed metadata
  auto metadata = ui.GetMetadata();
  ASSERT_TRUE(metadata->HasField(Constants::USER_CHOICES_TEXT_NAME));
  ASSERT_EQ((*metadata)[Constants::USER_CHOICES_TEXT_NAME].GetType(), sup::dto::StringType);
  EXPECT_EQ((*metadata)[Constants::USER_CHOICES_TEXT_NAME].As<std::string>(), main_text);
  ASSERT_TRUE(metadata->HasField(Constants::USER_CHOICES_DIALOG_TYPE_NAME));
  ASSERT_EQ((*metadata)[Constants::USER_CHOICES_DIALOG_TYPE_NAME].GetType(),
             sup::dto::UnsignedInteger32Type);
  EXPECT_EQ((*metadata)[Constants::USER_CHOICES_DIALOG_TYPE_NAME].As<sup::dto::uint32>(),
             dialog_type::kConfirmation);
}

TEST(UserConfirmation, VariableAttributes)
{
  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
  <UserConfirmation description="@descr" okText="@yes" cancelText="@no"/>
  <Workspace>
    <Local name="descr" type='{"type":"string"}' value='"Proceed"'/>
    <Local name="yes" type='{"type":"string"}' value='"yes"'/>
    <Local name="no" type='{"type":"string"}' value='"no"'/>
  </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  // zero indicates success branch
  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui));
}

TEST(UserConfirmation, VariableAttributesWrongType)
{
  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
  <UserConfirmation description="@descr" okText="@yes" cancelText="@no"/>
  <Workspace>
    <Local name="descr" type='{"type":"string"}' value='"Proceed"'/>
    <Local name="yes" type='{"type":"float32"}' value='4.3'/>
    <Local name="no" type='{"type":"string"}' value='"no"'/>
  </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  // zero indicates success branch
  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST(UserConfirmation, VariableAttributesNotPresent)
{
  sup::UnitTestHelper::MockUI ui;
  const std::string body{R"(
  <UserConfirmation description="@descr" okText="@yes" cancelText="@no"/>
  <Workspace>
    <Local name="descr" type='{"type":"string"}' value='"Proceed"'/>
    <Local name="yes" type='{"type":"string"}' value='"yes"'/>
  </Workspace>
)"};

  auto proc = ParseProcedureString(sup::UnitTestHelper::CreateProcedureString(body));
  ASSERT_TRUE(proc.get() != nullptr);

  // zero indicates success branch
  ui.SetChoice(0);
  EXPECT_TRUE(TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

