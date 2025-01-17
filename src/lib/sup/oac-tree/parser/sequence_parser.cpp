/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/oac-tree/sequence_parser.h>

#include "procedure_parser.h"
#include "procedure_to_treedata_utils.h"

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/generic_utils.h>

#include <sup/xml/tree_data_parser.h>
#include <sup/xml/tree_data_serialize.h>

namespace sup
{
namespace oac_tree
{
void LoadPlugin(const std::string& name)
{
  utils::LoadLibrary(name);
}

std::unique_ptr<Procedure> ParseProcedureFile(const std::string& filename)
{
  auto data = sup::xml::TreeDataFromFile(filename);

  if (!data)
  {
    std::string error_message =
      "ParseProcedureFile(): could not parse file [" + filename + "] to TreeData";
    throw ParseException(error_message);
  }

  return ParseProcedure(*data, filename);
}

std::unique_ptr<Procedure> ParseProcedureString(const std::string& xml_str)
{
  auto data = sup::xml::TreeDataFromString(xml_str);

  if (!data)
  {
    auto xml_head = xml_str.substr(0, 1024);
    std::string error_message =
      "ParseProcedureFile(): could not parse xml string to TreeData:\n" +
       xml_head + "\n...";
    throw ParseException(error_message);
  }

  return ParseProcedure(*data);
}

//! Returns XML string representing Procedure.
std::string GetXMLString(const Procedure& procedure)
{
  auto tree_data = ToTreeData(procedure);
  return sup::xml::TreeDataToString(*tree_data);
}

}  // namespace oac_tree

}  // namespace sup
