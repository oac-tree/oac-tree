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
static std::unique_ptr<InstructionData> ParseInstructionNode(xmlDocPtr doc, xmlNodePtr node);
static std::unique_ptr<WorkspaceData> ParseWorkspaceNode(xmlDocPtr doc, xmlNodePtr node);
static std::unique_ptr<VariableData> ParseVariableNode(xmlDocPtr doc, xmlNodePtr node);
static bool NodeHasName(xmlNodePtr node, const char * name);
static std::string ToString(const xmlChar * xml_name);

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
  if (xmlStrcmp(root_node->name, (const xmlChar *)"Procedure"))
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
    if (cur->type != XML_ELEMENT_NODE)
    {
      cur = cur->next;
      continue;
    }
    if (NodeHasName(cur, "Workspace"))
    {
      // TODO: test for duplicate 'workspace' elements
      ws_data = ParseWorkspaceNode(doc, cur);
    }
    else
    {
      // Every non workspace element of the Procedure node should be an instruction node
      if (!instr_data)
      {
        instr_data = ParseInstructionNode(doc, cur);
      }
      else
      {
        log_warning("ParseProcedureNode() - multiple root instructions are (not yet) supported!");
        return {};
      }
    }
    cur = cur->next;
  }

  if (ws_data)
  {
    auto n_vars = ws_data->GetVariableDataList().size();
    log_info("ParseProcedureNode() - workspace parsed with %d variables",
             n_vars);
  }

  if (instr_data && ws_data)
  {
    return std::unique_ptr<ProcedureData>(
             new ProcedureData(instr_data.release(), ws_data.release()));
  }

  return {};
}

static std::unique_ptr<InstructionData> ParseInstructionNode(xmlDocPtr doc, xmlNodePtr node)
{
  auto node_name = ToString(node->name);
  std::unique_ptr<InstructionData> result(new InstructionData(node_name));

  // Add attributes
  auto attribute = node->properties;
  while (attribute != nullptr)
  {
    auto name = ToString(attribute->name);
    auto xml_val = xmlGetProp(node, attribute->name);
    auto value = ToString(xml_val);
    xmlFree(xml_val);
    result->AddAttribute(name, value);
    attribute = attribute->next;
  }

  // Add children
  auto child_node = node->children;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_ELEMENT_NODE)
    {
      log_info("Add child InstructionData: %s", reinterpret_cast<const char *>(child_node->name));
      auto child_data = ParseInstructionNode(doc, child_node);
      result->AddChild(*child_data);
    }
    child_node = child_node->next;
  }

  return result;
}

static std::unique_ptr<WorkspaceData> ParseWorkspaceNode(xmlDocPtr doc, xmlNodePtr node)
{
  std::unique_ptr<WorkspaceData> result(new WorkspaceData());

  // Add child variables
  auto child_node = node->children;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_ELEMENT_NODE)
    {
      log_info("Add workspace variable with type '%s'", reinterpret_cast<const char *>(child_node->name));
      auto child_data = ParseVariableNode(doc, child_node);
      log_info("Added workspace variable with type '%s' and name '%s'",
               reinterpret_cast<const char *>(child_node->name),
               child_data->GetName().c_str());
      result->AddVariable(*child_data);
    }
    child_node = child_node->next;
  }

  return result;
}

static std::unique_ptr<VariableData> ParseVariableNode(xmlDocPtr doc, xmlNodePtr node)
{
  auto node_name = ToString(node->name);
  std::unique_ptr<VariableData> result(new VariableData(node_name));

  // Add attributes
  auto attribute = node->properties;
  while (attribute != nullptr)
  {
    auto name = ToString(attribute->name);
    auto xml_val = xmlGetProp(node, attribute->name);
    auto value = ToString(xml_val);
    xmlFree(xml_val);
    result->AddAttribute(name, value);
    attribute = attribute->next;
  }

  return result;
}

static bool NodeHasName(xmlNodePtr node, const char * name)
{
  return (xmlStrcmp(node->name, (const xmlChar *)name) == 0);
}

static std::string ToString(const xmlChar * xml_name)
{
  return std::string(reinterpret_cast<const char *>(xml_name), xmlStrlen(xml_name));
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
