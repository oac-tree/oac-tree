#include <libxml/parser.h>

#include "SequenceParser.h"

namespace
{
std::unique_ptr<SequenceParser::ProcedureData> ParseProcedureNode(xmlDocPtr doc, xmlNodePtr root);
std::unique_ptr<SequenceParser::InstructionData> ParseInstructionNode(xmlDocPtr doc, xmlNodePtr node);
std::unique_ptr<SequenceParser::WorkspaceData> ParseWorkspaceNode(xmlDocPtr doc, xmlNodePtr node);
}

namespace SequenceParser
{
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

} // namespace SequenceParser

namespace
{

std::unique_ptr<SequenceParser::ProcedureData> ParseProcedureNode(xmlDocPtr doc, xmlNodePtr root)
{
  std::unique_ptr<SequenceParser::InstructionData> instr_data;
  std::unique_ptr<SequenceParser::WorkspaceData> ws_data;
  xmlNodePtr cur = root->children;
  while (cur != nullptr)
  {
    if (!xmlStrcmp(cur->name, (const xmlChar *)"root"))
    {
      // TODO: test for duplicate 'root' elements
      instr_data = ParseInstructionNode(doc, cur);
    }
    else if (!xmlStrcmp(cur->name, (const xmlChar *)"workspace"))
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
    return std::unique_ptr<SequenceParser::ProcedureData>(
             new SequenceParser::ProcedureData(instr_data.release(), ws_data.release()));
  }

  return {};
}


std::unique_ptr<SequenceParser::InstructionData> ParseInstructionNode(xmlDocPtr doc, xmlNodePtr node)
{
  return std::unique_ptr<SequenceParser::InstructionData>(
           new SequenceParser::InstructionData());
}


std::unique_ptr<SequenceParser::WorkspaceData> ParseWorkspaceNode(xmlDocPtr doc, xmlNodePtr node)
{
  return std::unique_ptr<SequenceParser::WorkspaceData>(
           new SequenceParser::WorkspaceData());
}

} // unnamed namespace