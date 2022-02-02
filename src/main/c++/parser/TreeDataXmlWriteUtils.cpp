/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "TreeDataXmlWriteUtils.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "TreeData.h"
#include "XmlUtils.h"

#include <stdexcept>

// ----------------------------------------------------------------------------
// Declarations of helper methods and constants in anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
//! Main method for recursive writing of XML from TreeData.
void AddTreeData(xmlTextWriterPtr writer, const ::sup::sequencer::TreeData &tree_data);

//! Adds to currently opened XML element all attributes defined in TreeData.
void AddTreeAttributes(xmlTextWriterPtr writer, const ::sup::sequencer::TreeData &tree_data);

//! Set-up identation.
void SetupWriterIndentation(xmlTextWriterPtr writer);
}  // namespace

// ----------------------------------------------------------------------------
// Implementations declared in the header.
// ----------------------------------------------------------------------------

namespace sup
{
namespace sequencer
{
void WriteToXMLFile(const std::string &file_name, const TreeData &tree_data)
{
  xmlTextWriterPtr writer;

  // Create a new XmlWriter for uri, with no compression.
  writer = xmlNewTextWriterFilename(file_name.c_str(), 0);
  if (!writer)
    throw std::runtime_error("Error creating the XML writer");
  SetupWriterIndentation(writer);

  xmlTextWriterStartDocument(writer, nullptr, "UTF-8", nullptr);

  AddTreeData(writer, tree_data);

  xmlTextWriterEndDocument(writer);
  xmlFreeTextWriter(writer);
}

std::string GetXMLString(const TreeData &tree_data)
{
  xmlTextWriterPtr writer;
  xmlBufferPtr buf;

  // Create a new XML buffer, to which the XML document will be written
  buf = xmlBufferCreate();
  if (!buf)
    throw std::runtime_error("testXmlwriterMemory: Error creating the xml buffer");

  // Create a new XmlWriter for memory, with no compression.
  writer = xmlNewTextWriterMemory(buf, 0);
  if (!writer)
    throw std::runtime_error("testXmlwriterMemory: Error creating the xml writer");
  SetupWriterIndentation(writer);

  xmlTextWriterStartDocument(writer, nullptr, "UTF-8", nullptr);

  AddTreeData(writer, tree_data);

  xmlTextWriterEndDocument(writer);
  xmlFreeTextWriter(writer);

  return ToString(buf->content);
}
}  // namespace sequencer
}  // namespace sup

// ----------------------------------------------------------------------------
// Implementation of helper functions from anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
void AddTreeData(xmlTextWriterPtr writer, const ::sup::sequencer::TreeData &tree_data)
{
  using ::sup::sequencer::FromString;

  if (tree_data.GetType().empty())
    throw std::runtime_error("Error in AddTreeData: missed type in TreeData.");

  // opening element
  int rc = xmlTextWriterStartElement(writer, FromString(tree_data.GetType()));
  if (rc < 0)
    throw std::runtime_error("Error at xmlTextWriterStartElement");

  // writing attribute
  AddTreeAttributes(writer, tree_data);

  // writing content
  if (!tree_data.GetContent().empty())
  {
    rc = xmlTextWriterWriteString(writer, FromString(tree_data.GetContent()));
    if (rc < 0)
      throw std::runtime_error("Error in xmlTextWriterWriteString");
  }

  // writing children
  for (const auto &child : tree_data.Children())
  {
    AddTreeData(writer, child);
  }

  // closing element
  rc = xmlTextWriterEndElement(writer);
  if (rc < 0)
    throw std::runtime_error("Error at xmlTextWriterEndElement");
}

void AddTreeAttributes(xmlTextWriterPtr writer, const ::sup::sequencer::TreeData &tree_data)
{
  for (const auto &attr : tree_data.Attributes())
  {
    int rc = xmlTextWriterWriteAttribute(writer, ::sup::sequencer::FromString(attr.first), ::sup::sequencer::FromString(attr.second));
    if (rc < 0)
      throw std::runtime_error("Error at xmlTextWriterWriteAttribute");
  }
}

void SetupWriterIndentation(xmlTextWriterPtr writer)
{
  const int indentation_on = 1;
  xmlTextWriterSetIndent(writer, indentation_on);
  xmlTextWriterSetIndentString(writer, ::sup::sequencer::FromString(std::string("  ")));
}

}  // namespace
