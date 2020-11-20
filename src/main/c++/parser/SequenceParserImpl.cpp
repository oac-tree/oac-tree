/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : SUP - Sequencer
*
* Description   : Sequencer for operational procedures
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

#include <common/log-api.h>
#include <libxml/parser.h>

// Local header files

#include "SequenceParserImpl.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Unnamed namespace declarations

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

static std::unique_ptr<ProcedureData> ParseProcedureNode(xmlDocPtr doc, xmlNodePtr root);
static std::unique_ptr<InstructionData> ParseInstructionNode(xmlDocPtr doc, xmlNodePtr node,
                                                             bool is_root=false);
static std::unique_ptr<WorkspaceData> ParseWorkspaceNode(xmlDocPtr doc, xmlNodePtr node);
static bool NodeHasName(xmlNodePtr node, const char * name);

// Function definition

std::unique_ptr<ProcedureData> ParseProcedureXML(const char * const filename)
{
  // Read file into xmlDocPtr
  xmlDocPtr doc = xmlParseFile(filename);
  if (doc == nullptr)
  {
    return {};
  }

  // Check root element
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  if (root_node == nullptr)
  {
    xmlFreeDoc(doc);
    return {};
  }
  if (xmlStrcmp(root_node->name, (const xmlChar *)"procedure"))
  {
    xmlFreeDoc(doc);
    return {};
  }

  auto proc_data = ParseProcedureNode(doc, root_node);
  xmlFreeDoc(doc);
  return proc_data;
}

static std::unique_ptr<ProcedureData> ParseProcedureNode(xmlDocPtr doc, xmlNodePtr root)
{
  std::unique_ptr<InstructionData> instr_data;
  std::unique_ptr<WorkspaceData> ws_data;
  xmlNodePtr cur = root->children;
  while (cur != nullptr)
  {
    if (NodeHasName(cur, "root"))
    {
      // TODO: test for duplicate 'root' elements
      instr_data = ParseInstructionNode(doc, cur);
    }
    else if (NodeHasName(cur, "workspace"))
    {
      // TODO: test for duplicate 'workspace' elements
      ws_data = ParseWorkspaceNode(doc, cur);
    }
    else
    {
      // Undefined child
    }
    cur = cur->next;
  }

  if (instr_data && ws_data)
  {
    return std::unique_ptr<ProcedureData>(
             new ProcedureData(instr_data.release(), ws_data.release()));
  }

  return {};
}

static std::unique_ptr<InstructionData> ParseInstructionNode(xmlDocPtr doc, xmlNodePtr node,
                                                             bool is_root)
{
  // std::unique_ptr<InstructionData> result(new InstructionData());
  if (is_root)
  {
    if (!NodeHasName(node, "root"))
    {
      // issue
    }

  }

  return std::unique_ptr<InstructionData>(
           new InstructionData("DummyInstruction"));
}

static std::unique_ptr<WorkspaceData> ParseWorkspaceNode(xmlDocPtr doc, xmlNodePtr node)
{
  return std::unique_ptr<WorkspaceData>(
           new WorkspaceData());
}

static bool NodeHasName(xmlNodePtr node, const char * name)
{
  return (xmlStrcmp(node->name, (const xmlChar *)name) == 0);
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
