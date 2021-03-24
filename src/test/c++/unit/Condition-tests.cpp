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
#include "common/BasicTypes.h"
#include "common/AnyValue.h"
#include "common/SharedReference.h"
#include "common/AnyType.h"

// Local header files

#include "Workspace.h"
#include "LocalVariable.h"
#include "Condition.h"
#include "SequenceParser.h"

#include "UnitTestHelper.h"
#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type declaration

using namespace sup::sequencer;


static const ccs::types::char8 *conditionTable[][14] = {
        { "c", "{\"type\":\"uint8\"}", "0", "c", "false", NULL },
        { "c", "{\"type\":\"uint8\"}", "255", "c", "true", NULL },
        { "c", "{\"type\":\"uint64\"}", "10", "c", "true", NULL },
        { "c.field1", "{\"type\":\"StructuredData1\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}", "{\"field1\":2}", "c", "true", NULL },
        { "c[1].field1", "{\"type\":\"StructuredData6a\", \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}", "[{\"field1\":1}, {\"field1\":0}]", "c", "false", NULL },
        { NULL } };


// Function definition

static inline bool Initialise(void) {
    return true;
}

static inline bool Terminate(void) {
    return true;
}

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);

TEST(Condition, Default) // Static initialisation
{
    bool status = Initialise();

    auto proc = sup::sequencer::ParseProcedureFile("../resources/workspaceCondition.xml");

    status = bool(proc);

    if (status)
    {
        status = PrintProcedureWorkspace(proc.get());
    }

    if (status)
    {
        LogUI ui;
        proc->ExecuteSingle(&ui);
        status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
    }

    if (status)
    {
        status = PrintProcedureWorkspace(proc.get());
    }

    Terminate();

    ASSERT_EQ(true, status);
}


TEST(Condition, Default1) // Static initialisation
{
    bool status = Initialise();

    ccs::types::uint32 i=0u;
    while((conditionTable[i][0]!=NULL) && status) {

        std::unique_ptr<Procedure> proc(new Procedure);
        std::unique_ptr<Condition> myCondNode(new Condition);
        myCondNode->AddAttribute("var_name", conditionTable[i][0]);

        ccs::types::AnyValue resVal;

        std::unique_ptr<Variable> varX(new LocalVariable);
        varX->AddAttribute("type", conditionTable[i][1]);
        varX->AddAttribute("value", conditionTable[i][2]);

        varX->GetValue(resVal);
        proc->AddVariable(conditionTable[i][3], varX.release());

        status = PrintProcedureWorkspace(proc.get());

        if (status) {
            LogUI ui;
            proc->PushInstruction(myCondNode.release());
            proc->Setup();
            proc->ExecuteSingle(&ui);

            ::std::string result=conditionTable[i][4];
            if(result=="true"){
                status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
            }
            else{
                status = (proc->GetStatus() == ExecutionStatus::FAILURE);
            }
        }


        if (status) {
            status = PrintProcedureWorkspace(proc.get());
        }

        i++;
    }

    Terminate();

    ASSERT_EQ(true, status);
}

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure) {
    auto var_names = procedure->VariableNames();
    ::ccs::types::char8 val_string[1024];
    for (const auto &var_name : var_names) {
        ::ccs::types::AnyValue val;
        log_info("Variable '%s'", var_name.c_str());

        bool var_initialized = procedure->GetVariableValue(var_name, val);
        if (var_initialized) {
            val.SerialiseInstance(val_string, 1024);
            log_info("Variable '%s', with value\n  %s", var_name.c_str(), val_string);
        }
        else {
            log_info("Variable '%s' uninitialized", var_name.c_str());
        }
    }
    return true;
}

TEST(Condition, NonScalarVariable_success)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Condition name=\"struct\" var_name=\"struct.timestamp\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "        <Local name=\"struct\"\n"
    "               type='{\"type\":\"sup::test::MyType/v1.0\",\"attributes\":[{\"timestamp\":{\"type\":\"uint64\"}}]}'\n"
    "               value='{\"timestamp\":1}'/>"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui);

  ASSERT_EQ(true, status);

}

TEST(Condition, NonScalarVariable_failure)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Condition name=\"struct\" var_name=\"struct\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "        <Local name=\"struct\"\n"
    "               type='{\"type\":\"sup::test::MyType/v1.0\",\"attributes\":[{\"timestamp\":{\"type\":\"uint64\"}}]}'\n"
    "               value='{\"timestamp\":1}'/>"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

TEST(Condition, NoSuchVariable_name)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Condition name=\"struct\" var_name=\"undefined\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

TEST(Condition, NoSuchVariable_attr)
{

  sup::sequencer::LogUI ui;
  auto proc = sup::sequencer::ParseProcedureString(
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<Procedure xmlns=\"http://codac.iter.org/sup/sequencer\" version=\"1.0\"\n"
    "           name=\"Trivial procedure for testing purposes\"\n"
    "           xmlns:xs=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
    "           xs:schemaLocation=\"http://codac.iter.org/sup/sequencer sequencer.xsd\">\n"
    "    <Repeat maxCount=\"10\">\n"
    "        <Condition name=\"struct\" var_name=\"struct.array[0].node\"/>\n"
    "    </Repeat>\n"
    "    <Workspace>\n"
    "        <Local name=\"struct\"\n"
    "               type='{\"type\":\"sup::test::MyType/v1.0\",\"attributes\":[{\"timestamp\":{\"type\":\"uint64\"}}]}'\n"
    "               value='{\"timestamp\":1}'/>"
    "    </Workspace>\n"
    "</Procedure>");

  bool status = sup::UnitTestHelper::TryAndExecute(proc, &ui, sup::sequencer::ExecutionStatus::FAILURE);

  ASSERT_EQ(true, status);

}

#undef LOG_ALTERN_SRC
