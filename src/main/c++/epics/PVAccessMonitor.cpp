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

#include "Variable.h"
#include "VariableRegistry.h"

#include "LocalVariable.h"
#include "Workspace.h"


// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

class PVMonitorCache : public ccs::base::PVMonitor
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

  protected:

  public:

    /**
     * @brief Constructor.
     */

    PVMonitorCache (void);

    /**
     * @brief Destructor.
     */

    virtual ~PVMonitorCache (void);

    /**
     * @brief Accessor
     */

    bool IsInitialised (void) const;
    bool GetValue (ccs::types::AnyValue& value) const;

    bool SetChannel (const ccs::types::char8 * const name);

    /**
     * @brief See ccs::base::PVMonitor.
     */

    virtual void HandleEvent (const ccs::base::PVMonitor::Event& event);
    virtual void HandleMonitor (const ccs::types::AnyValue& value);

};

/**
 * @brief BlockingPVMonitorNode class.
 * @detail Blocking instruction which establishes a PVAccess connection and updates a
 * workspace variable based on monitor received. Mandatory attributes are 'channel' (PV name)
 * and 'variable' workspace variable name. The 'timeout' attribute is optional and defaulted
 * to 5s.
 */

class BlockingPVMonitorNode : public Instruction, public PVMonitorCache
{

  private:

    /**
     * @brief Timeout parameter.
     */

    ccs::types::uint64 _timeout = 5000000000ul;

    /**
     * @brief Setup using provided attributes.
     */

    virtual bool Setup (void);

    /**
     * @brief Xxx
     */

    ExecutionStatus ExecuteSingleImpl (UserInterface * ui, Workspace * ws) override;

  protected:

  public:

    /**
     * @brief Constructor.
     */

    BlockingPVMonitorNode (void);

    /**
     * @brief Destructor.
     */

    ~BlockingPVMonitorNode (void) override;

};

/**
 * @brief PVMonitorVariable class.
 * @detail Workspace variable with asynchronous PVAccess monitoring. Mandatory attribute is the
 * named 'channel' (PV name) to connect to.
 * @todo Implement access protection between PVMonitor::HandleMonitor and Variable::GetValueImpl.
 */

class PVMonitorVariable : public Variable, public PVMonitorCache
{

  private:

    /**
     * @brief Setup using provided attributes.
     */

    virtual bool Setup (void);

  protected:

  public:

    /**
     * @brief Constructor.
     */

    PVMonitorVariable (void);

    /**
     * @brief Destructor.
     */

    ~PVMonitorVariable (void) override;

    /**
     * @brief See sup::sequencer::Variable.
     */

    virtual bool GetValueImpl (ccs::types::AnyValue& value) const;
    virtual bool SetValueImpl (const ccs::types::AnyValue& value);

    static const std::string Type;

};

const std::string PVMonitorVariable::Type = "PVMonitorVariable";

// Function declaration

template <typename Type> inline Type ToInteger (const ccs::types::char8 * const buffer);

bool RegisterPVMonitor (void);

// Global variables

static bool global_pvmonitor_initialised_flag = RegisterPVMonitor();

// Function definition

template <> inline ccs::types::uint64 ToInteger (const ccs::types::char8 * const buffer)
{

  ccs::types::uint64 ret = 0ul;

  bool status = (ccs::HelperTools::IsIntegerString(buffer) == true);

  if (status)
    {
      ccs::types::char8* p = NULL_PTR_CAST(ccs::types::char8*);
      ret = static_cast<ccs::types::uint64>(strtoul(buffer, &p, 10));
    }

  return ret;

}

bool RegisterPVMonitor (void)
{

  { // PVMonitor instruction
    auto constructor = []() { return static_cast<Instruction*>(new BlockingPVMonitorNode ()); };
    GlobalInstructionRegistry().RegisterInstruction("BlockingPVMonitorNode", constructor);
  }

  { // PVMonitor variable
    auto constructor = []() { return static_cast<Variable*>(new PVMonitorVariable ()); };
    GlobalVariableRegistry().RegisterVariable(PVMonitorVariable::Type, constructor);
  }

  return true;

}

// cppcheck-suppress unusedFunction // Callbacks used in a separate translation unit
void PVMonitorCache::HandleEvent (const ccs::base::PVMonitor::Event& event)
{

  if (ccs::base::PVMonitor::Event::Connect == event)
    {
      log_notice("PVMonitorCache::HandleEvent - Connect to '%s'", ccs::base::PVMonitor::GetChannel());
    }

  if (_initialised && (ccs::base::PVMonitor::Event::Disconnect == event))
    {
      log_notice("PVMonitorCache::HandleEvent - Disconnect from '%s'", ccs::base::PVMonitor::GetChannel());
      _initialised = false;
    }

  return;

}

// cppcheck-suppress unusedFunction // Callbacks used in a separate translation unit
void PVMonitorCache::HandleMonitor (const ccs::types::AnyValue& value)
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
      ccs::types::char8 buffer [2048];
      (void)_value.SerialiseInstance(buffer, 2048u);
      log_info("PVMonitorCache::HandleMonitor - Storing '%s' value", buffer);
    }

  return;

}

bool PVMonitorCache::IsInitialised (void) const { return _initialised; }

bool PVMonitorCache::SetChannel (const ccs::types::char8 * const name)
{

  bool status = (false == _initialised);

  if (status)
    {
      status = ccs::base::PVMonitor::SetChannel(name);
    }

  if (status)
    { // Instantiate implementation
      status = ccs::base::PVMonitor::Initialise();
    }

  return status;

}

bool PVMonitorCache::GetValue (ccs::types::AnyValue& value) const
{

  bool status = _initialised;

  if (status)
    {
      value = _value;
    }

  return status;

}

bool BlockingPVMonitorNode::Setup (void)
{

  log_info("BlockingPVMonitorNode('%s')::Setup - Method called ..", GetName().c_str());

  bool status = (HasAttribute("channel") && HasAttribute("variable") && (false == PVMonitorCache::IsInitialised()));

  if (status)
    {
      log_info("BlockingPVMonitorNode('%s')::Setup - .. with channel '%s'", GetName().c_str(), GetAttribute("channel").c_str());
      log_info("BlockingPVMonitorNode('%s')::Setup - .. using workspace variable '%s'", GetName().c_str(), GetAttribute("variable").c_str());
    }

  if (status && HasAttribute("timeout"))
    { // Move to HelperTools namespace
      _timeout = ToInteger<ccs::types::uint64>(GetAttribute("timeout").c_str());
    }

  if (status)
    { // Instantiate implementation
      status = PVMonitorCache::SetChannel(GetAttribute("channel").c_str());
    }

  return status;

}

ExecutionStatus BlockingPVMonitorNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = PVMonitorCache::IsInitialised(); // Has received monitor and valid value

  if (!status)
    { // Instantiate implementation
      status = Setup();
    }

  ccs::types::AnyValue _value; // Placeholder

  while (status && (0ul < _timeout) && (false == PVMonitorCache::GetValue(_value)))
    {
      (void)ccs::HelperTools::SleepFor(10000000ul);

      if (10000000ul < _timeout)
        {
          _timeout -= 10000000ul;
        }
      else
        {
          _timeout = 0ul;
        }
    }

  if (status && static_cast<bool>(_value.GetType()))
    { // Verify if the named variable exists in the workspace
      log_info("BlockingPVMonitorNode('%s')::ExecuteSingleImpl - .. verify workspace content", GetName().c_str());
      if (ws->VariableNames().end() == std::find(ws->VariableNames().begin(), ws->VariableNames().end(), GetAttribute("variable").c_str()))
        { // .. create variable in the workspace but this requires the type
          log_info("BlockingPVMonitorNode('%s')::ExecuteSingleImpl - .. create '%s' variable in workspace", GetName().c_str(), GetAttribute("variable").c_str());
          LocalVariable* _variable = new (std::nothrow) LocalVariable (_value.GetType());
          status = ws->AddVariable(GetAttribute("variable"), _variable);
        }
    }

  if (status && static_cast<bool>(_value.GetType()))
    { // Write to workspace
      log_info("BlockingPVMonitorNode('%s')::ExecuteSingleImpl - .. update '%s' workspace variable", GetName().c_str(), GetAttribute("variable").c_str());
      status = ws->SetValue(GetAttribute("variable"), _value);
      //(void)ws->SetValue(GetAttribute("variable"), _value);
    }
  else
    {
      status = false;
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

bool PVMonitorVariable::Setup (void)
{

  bool status = ((false == PVMonitorCache::IsInitialised()) && Variable::HasAttribute("channel"));

  if (status)
    { // Instantiate implementation
      log_info("PVMonitorVariable('%s')::Setup - Method called with '%s' channel", GetName().c_str(), GetAttribute("channel").c_str());
      status = PVMonitorCache::SetChannel(GetAttribute("channel").c_str());
    }

  return status;

}

bool PVMonitorVariable::GetValueImpl (ccs::types::AnyValue& value) const
{

  bool status = PVMonitorCache::IsInitialised(); // Has received monitor and valid value

  // ToDo - MUTEX operation

  if (status)
    {
      PVMonitorCache::GetValue(value);
    }

  return status;

}

// Unsupported
bool PVMonitorVariable::SetValueImpl (const ccs::types::AnyValue& value) { return false; }

PVMonitorCache::PVMonitorCache (void) : ccs::base::PVMonitor() {}
PVMonitorCache::~PVMonitorCache (void) {}

BlockingPVMonitorNode::BlockingPVMonitorNode (void) : Instruction("BlockingPVMonitorNode"), PVMonitorCache() {}
BlockingPVMonitorNode::~BlockingPVMonitorNode (void) {}

PVMonitorVariable::PVMonitorVariable (void) : Variable(PVMonitorVariable::Type), PVMonitorCache() {}

PVMonitorVariable::~PVMonitorVariable (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
