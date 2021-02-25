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
#include <stdio.h>
#include <string.h>
// Local header files

#include "SequenceParserImpl.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

#define INCL_NODE_TYPE "IncludeNode"
#define INCL_NODE_PATH_ATTR "path"
#define NODE_NAME_ATTR "name"

// Unnamed namespace declarations

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function declaration

static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node);

static std::string ToString(const xmlChar *xml_name);

static std::unique_ptr<TreeData> GetNodeDataFromXml(std::string path);

static std::string GetXmlNodeName(xmlNodePtr node);

// Function definition

std::unique_ptr<TreeData> ParseXMLDataFile(const std::string & filename)
{
  // Read file into xmlDocPtr
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  if (doc == nullptr)
  {
    log_warning("ParseXMLDataFile('%s') - Couldn't parse file", filename.c_str());
    return {};
  }

  return ParseXMLDoc(doc);
}

std::unique_ptr<TreeData> ParseXMLDataString(const std::string & xml_str)
{
    // Read the string into xmlDocPtr
  xmlDocPtr doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(xml_str.c_str()));
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
    if (root_node == nullptr) {
        log_warning("ParseXMLData('%s') - Couldn't retrieve root element", filename.c_str());
        xmlFreeDoc(doc);
        return {};
    }
    if (xmlStrcmp(root_node->name, (const xmlChar*) "Procedure")) {
        log_warning("ParseXMLData('%s') - Root element is not 'Procedure'", filename.c_str());
        xmlFreeDoc(doc);
        return {};
    }
    auto data_tree = ParseDataTree(doc, root_node);
    xmlFreeDoc(doc);
    return data_tree;
}

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node) {
    auto node_name = ToString(node->name);
    std::unique_ptr<TreeData> result(new TreeData(node_name));

    // Add attributes
    auto attribute = node->properties;
    while (attribute != nullptr) {
        auto name = ToString(attribute->name);
        auto xml_val = xmlGetProp(node, attribute->name);
        auto value = ToString(xml_val);
        xmlFree(xml_val);
        result->AddAttribute(name, value);
        attribute = attribute->next;
    }

    // Add children
    auto child_node = node->children;
    while (child_node != nullptr) {
        if (child_node->type == XML_TEXT_NODE) {
            auto xml_content = xmlNodeListGetString(doc, child_node, 1);
            auto content = ToString(xml_content);
            result->SetContent(content);
            xmlFree(xml_content);
        }
        if (child_node->type == XML_ELEMENT_NODE) {
            log_info("Add child Data: %s", reinterpret_cast<const char*>(child_node->name));

            std::string nodeName = ToString(child_node->name);
            if (nodeName == INCL_NODE_TYPE) {
                std::unique_ptr<TreeData> incl_data;
                auto attribute = child_node->properties;
                while (attribute != nullptr) {
                    auto attr_name = ToString(attribute->name);

                    if (attr_name == INCL_NODE_PATH_ATTR) {
                        auto xml_val = ToString(xmlGetProp(child_node, attribute->name));
                        auto incl_data = GetNodeDataFromXml(xml_val);
                        if (incl_data) {
                            result->AddChild(*incl_data);
                        }
                        break;
                    }
                    attribute = attribute->next;
                }
            }
            else {
                auto child_data = ParseDataTree(doc, child_node);
                result->AddChild(*child_data);
            }
        }
        child_node = child_node->next;
    }

    return result;
}


static std::string ToString(const xmlChar *xml_name) {
    return std::string(reinterpret_cast<const char*>(xml_name), xmlStrlen(xml_name));
}

static std::unique_ptr<TreeData> GetNodeDataFromXml(std::string path) {

    std::vector < ccs::types::char8 > buffer(path.c_str(), path.c_str() + path.size() + 1);

    std::string filename;

    ::ccs::types::uint32 i = 0u;
    while ((buffer[i] == '.') || buffer[i] == '/') {
        filename += buffer[i];
        i++;
    }
    auto token = strtok(&buffer[i], ".");
    filename += token;
    filename += ".xml";

    log_info("GetNodeDataFromXml - Parse file %s", filename.c_str());

    xmlDocPtr doc = xmlParseFile(filename.c_str());
    if (doc == nullptr) {
        log_warning("xmlParseFile('%s') - Couldn't parse file", filename.c_str());
        return {};
    }

    // Check root element
    xmlNodePtr node = xmlDocGetRootElement(doc);

    //include the whole tree!
    bool found = true;

    while (token != NULL) {
        token = strtok(NULL, ".");
        if (token != NULL) {
            auto child_node = node->children;
            found = false;
            while ((child_node != nullptr) && (!found)) {
                if (child_node->type == XML_ELEMENT_NODE) {
                    auto nodename = GetXmlNodeName(child_node);
                    found = (!nodename.empty());
                    if (found) {
                        found = (nodename == token);
                    }
                    if (found) {
                        node = child_node;
                    }
                }
                child_node = child_node->next;
            }
            //not found by name... try by type
            child_node = node->children;
            while ((child_node != nullptr) && (!found)) {
                if (child_node->type == XML_ELEMENT_NODE) {
                    auto nodename = GetXmlNodeName(child_node);
                    auto nodetype = ToString(child_node->name);
                    found = (nodename.empty());
                    if (found) {
                        found = (nodetype == token);
                    }
                    if (found) {
                        node = child_node;
                    }
                }
                child_node = child_node->next;
            }
        }
    }

    if (found) {
        log_info("ParseDataTree - Node %s included", path.c_str());
        return ParseDataTree(doc, node);
    }
    else {
        log_error("ParseDataTree - IncludeNode %s not found", path.c_str());
    }

    return {};
}

static std::string GetXmlNodeName(xmlNodePtr node) {
    auto attribute = node->properties;
    std::string ret;
    while ((attribute != nullptr) && ret.empty()) {
        auto name = ToString(attribute->name);
        if (name == NODE_NAME_ATTR) {
            auto xml_val = xmlGetProp(node, attribute->name);
            ret = ToString(xml_val);
        }
        attribute = attribute->next;
    }
    return ret;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
