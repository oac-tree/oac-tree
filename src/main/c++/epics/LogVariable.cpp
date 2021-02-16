/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : CODAC Supervision and Automation (SUP) Sequencer component
*
* Description   : Instruction node implementation
*
* Author        : Bertrand Bauvir (IO)
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

#include <new> // std::nothrow, etc.

#include <common/BasicTypes.h> // Misc. type definition
#include <common/StringTools.h> // Misc. helper functions

#include <common/AnyValueHelper.h>

#include <common/log-api.h> // Syslog wrapper routines

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"

#include "Workspace.h"
#include "Variable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

/**
 * @brief Xxx
 */

class LogVariableNode : public Instruction
{

  private:

    /**
     * @brief Xxx
     */

    ExecutionStatus ExecuteSingleImpl (UserInterface * ui, Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    LogVariableNode (void);

    /**
     * @brief Destructor.
     */

    ~LogVariableNode (void) override;

};

// Function declaration

bool RegisterInstruction_LogVariable (void);

// Global variables

static bool global_logvariable_initialised_flag = RegisterInstruction_LogVariable();

// Function definition

bool RegisterInstruction_LogVariable (void)
{

  auto constructor = []() { return static_cast<Instruction*>(new LogVariableNode ()); };
  GlobalInstructionRegistry().RegisterInstruction("LogVariableNode", constructor);

  return true;

}

ExecutionStatus LogVariableNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = HasAttribute("variable");

  if (status)
    {
      log_info("LogVariableNode('%s')::ExecuteSingleImpl - Method called with workspace variable '%s' ..", GetName().c_str(), GetAttribute("variable").c_str());
    }

  ccs::types::AnyValue _value;

  if (status)
    { // Read from workspace
      log_info("LogVariableNode('%s')::ExecuteSingleImpl - .. read '%s' workspace variable", GetName().c_str(), GetAttribute("variable").c_str());
      status = ws->GetValue(GetAttribute("variable"), _value);
    }

  if (status)
    {
      ccs::types::char8 buffer [2048];
      (void)ccs::HelperTools::SerialiseToJSONStream(&_value, buffer, 2048u);
      log_info("LogVariableNode('%s')::ExecuteSingleImpl - .. '%s'", Instruction::GetName().c_str(), buffer);
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

LogVariableNode::LogVariableNode (void) : Instruction("LogVariableNode") {}
LogVariableNode::~LogVariableNode (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
