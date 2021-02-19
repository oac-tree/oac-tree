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
#include <common/TimeTools.h> // Misc. helper functions

#include <common/log-api.h> // Syslog wrapper routines

#include <common/RPCClient.h> // Plug-in managed through dynamic linking .. see Makefile

// Local header files

#include "Instruction.h"
#include "InstructionRegistry.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

/**
 * @brief Xxx
 */

class BlockingRPCClientNode : public Instruction
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

    BlockingRPCClientNode (void);

    /**
     * @brief Destructor.
     */

    ~BlockingRPCClientNode (void) override;

};

// Function declaration

bool RegisterInstruction_RPCClient (void);

// Global variables

static bool global_rpcclient_initialised_flag = RegisterInstruction_RPCClient();

// Function definition

bool RegisterInstruction_RPCClient (void)
{
#if 0 // Requires class name as public member
  RegisterGlobalInstruction<BlockingRPCClientNode>();
#else
  log_info("RegisterInstruction_RPCClient - Entering function");
  auto constructor = []() { return static_cast<Instruction*>(new BlockingRPCClientNode ()); };
  GlobalInstructionRegistry().RegisterInstruction("BlockingRPCClientNode", constructor);
#endif
  return true;

}

ExecutionStatus BlockingRPCClientNode::ExecuteSingleImpl (UserInterface * ui, Workspace * ws)
{

  (void)ui;
  (void)ws;

  if (HasAttribute("service"))
    {
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - Method called with service '%s' ..", GetName().c_str(), GetAttribute("service").c_str());
    }
  else
    {
      AddAttribute("service", "rpc@echo");
    }

  if (HasAttribute("datatype"))
    {
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - .. type '%s'", GetName().c_str(), GetAttribute("datatype").c_str());
    }
  else
    {
      AddAttribute("datatype", "{\"type\":\"rpcStruct\",\"attributes\":[{\"value\":{\"type\":\"uint64\", \"size\":8}}]}");
    }

  if (HasAttribute("instance"))
    {
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - .. instance '%s'", GetName().c_str(), GetAttribute("instance").c_str());
    }
  else
    {
      AddAttribute("instance", "{\"value\":0}");
    }

  // Create RPC client
  ccs::base::RPCClient client (GetAttribute("service").c_str());

  // Instantiate request from type
  ccs::types::AnyValue request (GetAttribute("datatype").c_str());

  // Parse request instance from stream
  bool status = request.ParseInstance(GetAttribute("instance").c_str());

  if (status)
    {
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - Launch ..", GetName().c_str());
      status = client.Launch();
    }
#if 0
  if (status)
    {
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - Test connection ..", GetName().c_str());
      status = client.IsConnected();
    }
#endif
  ccs::types::AnyValue reply;

  if (status)
    {
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - Send request ..", GetName().c_str());
      reply = client.SendRequest(request);
      status = static_cast<bool>(reply.GetType());
    }

  if (status && ccs::HelperTools::HasAttribute(&reply, "status"))
    {
      status = ccs::HelperTools::GetAttributeValue<bool>(&reply, "status");
    }

  if (status)
    {
      ccs::types::char8 buffer [1024];

      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - Received reply ..", GetName().c_str());
      reply.SerialiseType(buffer, 1024u);
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - .. type '%s'", GetName().c_str(), buffer);
      reply.SerialiseInstance(buffer, 1024u);
      log_info("BlockingRPCClientNode::ExecuteSingleImpl('%s') - .. instance '%s'", GetName().c_str(), buffer);
    }

  return (status ? ExecutionStatus::SUCCESS : ExecutionStatus::FAILURE);

}

BlockingRPCClientNode::BlockingRPCClientNode (void) : Instruction("BlockingRPCClientNode") {}
BlockingRPCClientNode::~BlockingRPCClientNode (void) {}

} // namespace sequencer

} // namespace sup

#undef LOG_ALTERN_SRC
