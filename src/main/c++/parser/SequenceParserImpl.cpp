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
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <common/SysTools.h>
#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>

#include <common/log-api.h>

// Local header files

#include "SequenceParserImpl.h"
#include "XmlUtils.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Unnamed namespace declarations

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node);

static void AddXMLAttributes(TreeData *tree, xmlNodePtr node);

static void AddXMLChildren(TreeData *tree, xmlDocPtr doc, xmlNodePtr node);

// Function definition

std::unique_ptr<TreeData> ParseXMLDataFile(const std::string &filename)
{
  // Read file into xmlDocPtr
  if (!::ccs::HelperTools::Exist(filename.c_str()))
  {
    log_warning("ParseXMLDataFile('%s') - file not found", filename.c_str());
    return {};
  }
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  if (doc == nullptr)
  {
    log_warning("ParseXMLDataFile('%s') - Couldn't parse file", filename.c_str());
    return {};
  }
  return ParseXMLDoc(doc);
}

std::unique_ptr<TreeData> ParseXMLDataString(const std::string &xml_str)
{
  // Read the string into xmlDocPtr
  xmlDocPtr doc = xmlParseDoc(reinterpret_cast<const xmlChar *>(xml_str.c_str()));
  auto xml_head = xml_str.substr(0, 1024);
  if (doc == nullptr)
  {
    log_warning("ParseXMLDataString('%s') - Couldn't parse file", xml_head.c_str());
    return {};
  }
  return ParseXMLDoc(doc);
}

static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc)
{
  // Check root element
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  if (root_node == nullptr)
  {
    log_warning("ParseXMLDoc() - Couldn't retrieve root element");
    xmlFreeDoc(doc);
    return {};
  }
  auto data_tree = ParseDataTree(doc, root_node);
  xmlFreeDoc(doc);
  return data_tree;
}

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  auto node_name = ToString(node->name);
  std::unique_ptr<TreeData> result(new TreeData(node_name));

  AddXMLAttributes(result.get(), node);

  AddXMLChildren(result.get(), doc, node);

  return result;
}

static void AddXMLAttributes(TreeData *tree, xmlNodePtr node)
{
  auto attribute = node->properties;
  while (attribute != nullptr)
  {
    auto name = ToString(attribute->name);
    auto xml_val = xmlGetProp(node, attribute->name);
    auto value = ToString(xml_val);
    xmlFree(xml_val);
    tree->AddAttribute(name, value);
    attribute = attribute->next;
  }
}

static void AddXMLChildren(TreeData *tree, xmlDocPtr doc, xmlNodePtr node)
{
  auto child_node = node->children;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_TEXT_NODE)
    {
      auto xml_content = xmlNodeListGetString(doc, child_node, 1);
      auto content = ToString(xml_content);
      tree->SetContent(content);
      xmlFree(xml_content);
    }
    else if (child_node->type == XML_ELEMENT_NODE)
    {
      log_debug("Add child Data: %s", reinterpret_cast<const char *>(child_node->name));
      auto child_data = ParseDataTree(doc, child_node);
      tree->AddChild(*child_data);
    }
    child_node = child_node->next;
  }
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C

#undef LOG_ALTERN_SRC
