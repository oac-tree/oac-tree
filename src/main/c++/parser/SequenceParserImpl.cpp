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

static std::unique_ptr<XMLData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node);

static bool NodeHasName(xmlNodePtr node, const char * name);
static std::string ToString(const xmlChar * xml_name);

// Function definition

std::unique_ptr<XMLData> ParseXMLData(const char * const filename)
{
  // Read file into xmlDocPtr
  xmlDocPtr doc = xmlParseFile(filename);
  if (doc == nullptr)
  {
    log_warning("ParseXMLData('%s') - Couldn't parse file", filename);
    return {};
  }

  // Check root element
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  if (root_node == nullptr)
  {
    log_warning("ParseXMLData('%s') - Couldn't retrieve root element", filename);
    xmlFreeDoc(doc);
    return {};
  }
  if (xmlStrcmp(root_node->name, (const xmlChar *)"Procedure"))
  {
    log_warning("ParseXMLData('%s') - Root element is not 'Procedure'", filename);
    xmlFreeDoc(doc);
    return {};
  }
  auto data_tree = ParseDataTree(doc, root_node);
  xmlFreeDoc(doc);
  return data_tree;
}

static std::unique_ptr<XMLData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  auto node_name = ToString(node->name);
  std::unique_ptr<XMLData> result(new XMLData(node_name));

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
      log_info("Add child Data: %s", reinterpret_cast<const char *>(child_node->name));
      auto child_data = ParseDataTree(doc, child_node);
      result->AddChild(*child_data);
    }
    child_node = child_node->next;
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
