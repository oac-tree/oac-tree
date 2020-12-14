/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project	: CODAC Supervision and Automation (SUP) Sequencer component
*
* Description	: Instruction node implementation
*
* Author        : Bertrand Bauvir (IO)
*
* Copyright (c) : 2010-2020 ITER Organization,
*				  CS 90 046
*				  13067 St. Paul-lez-Durance Cedex
*				  France
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
#include <common/TimeTools.h> // Misc. helper functions

#include <common/log-api.h> // Syslog wrapper routines

//#include <common/ChannelAccessClient.h>
#include <common/ChannelAccessHelper.h> // CA helper routines

// Local header files

#include "instructions/Instruction.h"
#include "instructions/InstructionRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

/**
 * @brief Xxx
 */

class BlockingCAWriteNode : public Instruction
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

    BlockingCAWriteNode (void);

    /**
     * @brief Destructor.
     */

    ~BlockingCAWriteNode (void) override;

};

// Function declaration

bool RegisterInstruction_ChannelAccessClient (void);

// Global variables

static ccs::log::Func_t __handler = ccs::log::SetStdout();

static bool global_caclient_initialised_flag = RegisterInstruction_ChannelAccessClient();

// Function definition

bool RegisterInstruction_ChannelAccessClient (void)
{

  log_info("RegisterInstruction_ChannelAccessClient - Entering function");

  auto constructor = []() { return static_cast<Instruction*>(new BlockingCAWriteNode ()); };
  GlobalInstructionRegistry().RegisterInstruction("BlockingCAWriteNode", constructor);

  return true;

}

ExecutionStatus BlockingCAWriteNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  if (HasAttribute("channel"))
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - Method called with service '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
    }
  else
    {
      AddAttribute("service", "SEQ-TEST:STRING");
    }

  if (HasAttribute("datatype"))
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - .. type '%s'", GetName().c_str(), GetAttribute("datatype").c_str());
    }
  else
    {
      AddAttribute("datatype", "string");
    }

  if (HasAttribute("instance"))
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - .. instance '%s'", GetName().c_str(), GetAttribute("instance").c_str());
    }
  else
    {
      AddAttribute("instance", "undefined");
    }

  // Create CA context
  bool status = ccs::HelperTools::ChannelAccess::CreateContext();

  chid channel = 0;

  if (status)
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - Connect to variable '%s' ..", GetName().c_str(), GetAttribute("channel").c_str());
      //status = ccs::HelperTools::ChannelAccess::ConnectVariable(GetAttribute("channel").c_str(), channel);
      (void)ccs::HelperTools::ChannelAccess::ConnectVariable(GetAttribute("channel").c_str(), channel);
    }

  if (status && ccs::HelperTools::ChannelAccess::IsConnected(channel))
    {
      log_info("BlockingCAWriteNode::ExecuteSingleImpl('%s') - Write as string '%s' ..", GetName().c_str(), GetAttribute("instance").c_str());
      status = ccs::HelperTools::ChannelAccess::WriteVariable(channel, DBR_STRING, const_cast<void*>(static_cast<const void*>(GetAttribute("instance").c_str())));
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

BlockingCAWriteNode::BlockingCAWriteNode (void) : Instruction("BlockingCAWriteNode") {}
BlockingCAWriteNode::~BlockingCAWriteNode (void) {}

} // namespace sequencer

} // namespace sup  

#undef LOG_ALTERN_SRC
