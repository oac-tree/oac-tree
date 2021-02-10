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
#include <algorithm> // std::find, etc.

#include <common/BasicTypes.h> // Misc. type definition
#include <common/StringTools.h> // Misc. helper functions
#include <common/TimeTools.h> // Misc. helper functions

#include <common/log-api.h> // Syslog wrapper routines

#include <common/PVMonitor.h> // Plug-in managed through dynamic linking .. see Makefile

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"

#include "Workspace.h"
#include "Variable.h"
#include "LocalVariable.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

/**
 * @brief Xxx
 */

class PVMonitorNode : public Instruction, public ccs::base::PVMonitor
{

  private:

    /**
     * @brief Intialised flag.
     */

    ccs::types::boolean _initialised = false;

    /**
     * @brief Workspace variable copy.
     */

    ccs::types::AnyValue _value;

    /**
     * @brief Xxx
     */

    ExecutionStatus ExecuteSingleImpl (UserInterface * ui, Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    PVMonitorNode (void);

    /**
     * @brief Destructor.
     */

    ~PVMonitorNode (void) override;

    /**
     * @brief See ccs::base::PVMonitor.
     */

    virtual void HandleEvent (const ccs::base::PVMonitor::Event& event);
    virtual void HandleMonitor (const ccs::types::AnyValue& value);

};

// Function declaration

bool RegisterInstruction_PVMonitor (void);

// Global variables

static ccs::log::Func_t __handler = ccs::log::SetStdout();

static bool global_pvmonitor_initialised_flag = RegisterInstruction_PVMonitor();

// Function definition

bool RegisterInstruction_PVMonitor (void)
{

  log_info("RegisterInstruction_PVMonitor - Entering method");

  auto constructor = []() { return static_cast<Instruction*>(new PVMonitorNode ()); };
  GlobalInstructionRegistry().RegisterInstruction("PVMonitorNode", constructor);

  return true;

}

// cppcheck-suppress unusedFunction // Callbacks used in a separate translation unit
void PVMonitorNode::HandleEvent (const ccs::base::PVMonitor::Event& event)
{

  if (ccs::base::PVMonitor::Event::Connect == event)
    {
      log_notice("PVMonitorNode('%s')::HandleEvent - Connect to '%s'", Instruction::GetName().c_str(), ccs::base::PVMonitor::GetChannel());
    }

  if (_initialised && (ccs::base::PVMonitor::Event::Disconnect == event))
    {
      log_notice("PVMonitorNode('%s')::HandleEvent - Disconnect from '%s'", Instruction::GetName().c_str(), ccs::base::PVMonitor::GetChannel());
      _initialised = false;
    }

  return;

}

// cppcheck-suppress unusedFunction // Callbacks used in a separate translation unit
void PVMonitorNode::HandleMonitor (const ccs::types::AnyValue& value)
{

  bool status = static_cast<bool>(value.GetType());

  if (status)
    {
      _initialised = true;
    }

  if (status)
    { // Copy variable
      _value = value;
    }

  if (status)
    {
      ccs::types::string buffer;
      (void)_value.SerialiseInstance(buffer, ccs::types::MaxStringLength);
      log_info("PVMonitorNode('%s')::HandleMonitor - Storing '%s' value", Instruction::GetName().c_str(), buffer);
    }

  return;

}

ExecutionStatus PVMonitorNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = (HasAttribute("channel") && HasAttribute("variable"));

  if (status)
    {
      log_info("PVMonitorNode('%s')::ExecuteSingleImpl - Method called with channel '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
      log_info("PVMonitorNode('%s')::ExecuteSingleImpl - .. using workspace variable '%s'", GetName().c_str(), GetAttribute("variable").c_str());
    }

  if (status && (false == _initialised))
    {
      // Instantiate implementation
      ccs::base::PVMonitor::SetChannel(GetAttribute("channel").c_str());
      ccs::base::PVMonitor::Initialise();

      // This is an asynchronous node .. for now let the implementation initialise
      (void)ccs::HelperTools::SleepFor(1000000000ul);
    }

  if (status && _initialised)
    { // Verify if the named variable exists in the workspace
      log_info("PVMonitorNode('%s')::ExecuteSingleImpl - .. verify workspace content", GetName().c_str());
      if (ws->VariableNames().end() == std::find(ws->VariableNames().begin(), ws->VariableNames().end(), GetAttribute("variable").c_str()))
        { // .. create variable in the workspace but this requires the type
          log_info("PVMonitorNode('%s')::ExecuteSingleImpl - .. create '%s' variable in workspace", GetName().c_str(), GetAttribute("variable").c_str());
          LocalVariable* _variable = new (std::nothrow) LocalVariable (_value.GetType());
          status = ws->AddVariable(GetAttribute("variable"), _variable);
        }
    }

  if (status && _initialised)
    { // Write to workspace
      log_info("PVMonitorNode('%s')::ExecuteSingleImpl - .. update '%s' workspace variable", GetName().c_str(), GetAttribute("variable").c_str());
      status = ws->SetValue(GetAttribute("variable"), _value);
      //(void)ws->SetValue(GetAttribute("variable"), _value);
    }
  else
    {
      status = false;
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

PVMonitorNode::PVMonitorNode (void) : Instruction("PVMonitorNode"), ccs::base::PVMonitor() {}
PVMonitorNode::~PVMonitorNode (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
