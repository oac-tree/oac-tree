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
*                                 CS 90 046
*                                 13067 St. Paul-lez-Durance Cedex
*                                 France
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

#include <common/AnyValue.h>
#include <common/AnyValueHelper.h>

//#include <common/ChannelAccessClient.h>
#include <common/ChannelAccessHelper.h> // CA helper routines
#include <common/AnyTypeToCA.h> // CA helper routines .. type conversion

// Local header files

#include "ExecutionStatus.h"

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
 * @todo Manage EPICS CA context as singleton and destroy when not necessary anylonger.
 * @todo Re-design to manage commonalities in separate class.
 */

class BlockingCAFetchNode : public Instruction
{

  private:

    /**
     * @brief CA channel identifier attribute.
     */

    chid _channel;

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

    BlockingCAFetchNode (void);

    /**
     * @brief Destructor.
     */

    ~BlockingCAFetchNode (void) override;

};

class BlockingCAWriteNode : public Instruction
{

  private:

    /**
     * @brief CA channel identifier attribute.
     */

    chid _channel;

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

    BlockingCAWriteNode (void);

    /**
     * @brief Destructor.
     */

    ~BlockingCAWriteNode (void) override;

};

// Function declaration

bool RegisterInstruction_ChannelAccessClient (void);

// Global variables

static bool global_caclient_initialised_flag = RegisterInstruction_ChannelAccessClient();

// Function definition

bool RegisterInstruction_ChannelAccessClient (void)
{

  { // CAFetch registration
    auto constructor = []() { return static_cast<Instruction*>(new BlockingCAFetchNode ()); };
    GlobalInstructionRegistry().RegisterInstruction("BlockingCAFetchNode", constructor);
  }

  { // CAWrite registration
    auto constructor = []() { return static_cast<Instruction*>(new BlockingCAWriteNode ()); };
    GlobalInstructionRegistry().RegisterInstruction("BlockingCAWriteNode", constructor);
  }

  return true;

}

ExecutionStatus BlockingCAFetchNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = (HasAttribute("channel") && HasAttribute("variable"));

  if (status)
    {
      log_info("BlockingCAFetchNode::ExecuteSingleImpl('%s') - Method called with channel '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
      log_info("BlockingCAFetchNode::ExecuteSingleImpl('%s') - .. using workspace variable '%s'", GetName().c_str(), GetAttribute("variable").c_str());

      // Verify if the named variable exists in the workspace
      if (ws->VariableNames().end() != std::find(ws->VariableNames().begin(), ws->VariableNames().end(), GetAttribute("variable").c_str()))
        { // .. in order to access the expected datatype
          status = ws->GetValue(GetAttribute("variable"), _value);
        }
      else if (HasAttribute("datatype"))
        {
          _value = ccs::types::AnyValue (GetAttribute("datatype").c_str());
          // ToDo - Create variable in the workspace
          status = static_cast<bool>(_value.GetType());
        }
      else
        {
          status = false;
        }
    }

  if (status)
    { // Create CA context
      status = ccs::HelperTools::ChannelAccess::CreateContext();
    }

  if (status)
    {
      log_info("BlockingCAFetchNode::ExecuteSingleImpl('%s') - Connect to variable '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
      //status = ccs::HelperTools::ChannelAccess::ConnectVariable(GetAttribute("channel").c_str(), _channel);
      (void)ccs::HelperTools::ChannelAccess::ConnectVariable(GetAttribute("channel").c_str(), _channel);
    }

  if (status && ccs::HelperTools::ChannelAccess::IsConnected(_channel))
    {
      log_info("BlockingCAFetchNode::ExecuteSingleImpl('%s') - Fetch as type '%s' ..", GetName().c_str(), _value.GetType()->GetName());
      status = ccs::HelperTools::ChannelAccess::ReadVariable(_channel, ccs::HelperTools::AnyTypeToCAScalar(_value.GetType()), _value.GetInstance());
    }

  if (status)
    { // Write to workspace
      //status = ws->SetValue(GetAttribute("variable"), _value);
      (void)ws->SetValue(GetAttribute("variable"), _value);
    }

  if (status && ws->GetValue(GetAttribute("variable"), _value))
    {
      ccs::types::string buffer;
      _value.SerialiseInstance(buffer, ccs::types::MaxStringLength);
      log_info("BlockingCAFetchNode::ExecuteSingleImpl('%s') - .. '%s' value in the workspace", GetName().c_str(), buffer);
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

ExecutionStatus BlockingCAWriteNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  bool status = HasAttribute("channel");

  if (status)
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - Method called with channel '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
      status = ((HasAttribute("variable") && (ws->VariableNames().end() != std::find(ws->VariableNames().begin(), ws->VariableNames().end(), GetAttribute("variable").c_str()))) ||
                (HasAttribute("datatype") && HasAttribute("instance")));
    }

  if (status)
    {
      if (HasAttribute("variable"))
        {
          log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - .. using workspace variable '%s'", GetName().c_str(), GetAttribute("variable").c_str());
          status = ws->GetValue(GetAttribute("variable"), _value);
        }
      else
        {
          log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - .. using type '%s'", GetName().c_str(), GetAttribute("datatype").c_str());
          log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - .. and instance '%s'", GetName().c_str(), GetAttribute("instance").c_str());
          _value = ccs::types::AnyValue (GetAttribute("datatype").c_str());
          status = _value.ParseInstance(GetAttribute("instance").c_str());
        }
    }

  if (status)
    { // Create CA context
      status = ccs::HelperTools::ChannelAccess::CreateContext();
    }

  if (status)
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - Connect to variable '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
      //status = ccs::HelperTools::ChannelAccess::ConnectVariable(GetAttribute("channel").c_str(), _channel);
      (void)ccs::HelperTools::ChannelAccess::ConnectVariable(GetAttribute("channel").c_str(), _channel);
    }

  if (status && ccs::HelperTools::ChannelAccess::IsConnected(_channel))
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - Write as type '%s' ..", GetName().c_str(), _value.GetType()->GetName());
      status = ccs::HelperTools::ChannelAccess::WriteVariable(_channel, ccs::HelperTools::AnyTypeToCAScalar(_value.GetType()), _value.GetInstance());
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

BlockingCAFetchNode::BlockingCAFetchNode (void) : Instruction("BlockingCAFetchNode") {}
BlockingCAFetchNode::~BlockingCAFetchNode (void) {}

BlockingCAWriteNode::BlockingCAWriteNode (void) : Instruction("BlockingCAWriteNode") {}
BlockingCAWriteNode::~BlockingCAWriteNode (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
