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

#include <sup/sequencer/sequence_parser.h>

#include "procedure_parser.h"
#include "sequence_parser_impl.h"
#include "treedata_xml_write_utils.h"
#include "procedure_to_treedata_utils.h"

#include <sup/sequencer/log.h>

#include <common/SysTools.h>

namespace sup
{
namespace sequencer
{
bool LoadPlugin(const std::string& name)
{
  log::Debug("sup::sequencer::LoadPlugin('%s') - trying to load plugin..", name.c_str());
  bool status = ::ccs::HelperTools::LoadSharedLibrary(name.c_str());

  if (!status)
  {
    log::Warning("sup::sequencer::LoadPlugin('%s') - could not load plugin", name.c_str());
  }
  else
  {
    log::Debug("sup::sequencer::LoadPlugin('%s') - successfully loaded plugin", name.c_str());
  }
  return status;
}

std::unique_ptr<Procedure> ParseProcedureFile(const std::string& filename)
{
  log::Debug("sup::sequencer::ParseProcedureFile('%s') - load file..", filename.c_str());
  auto data = ParseXMLDataFile(filename);

  if (!data)
  {
    log::Warning("sup::sequencer::ParseProcedureFile('%s') - could not parse file",
                filename.c_str());
    return {};
  }

  auto proc = ParseProcedure(*data, filename);

  if (!proc)
  {
    log::Warning("sup::sequencer::ParseProcedureFile('%s') - could not parse structure in file!",
                filename.c_str());
  }

  return proc;
}

std::unique_ptr<Procedure> ParseProcedureString(const std::string& xml_str)
{
  auto xml_head = xml_str.substr(0, 1024);
  log::Debug("sup::sequencer::ParseProcedureString('%s') - parsing string..", xml_head.c_str());
  auto data = ParseXMLDataString(xml_str);

  if (!data)
  {
    log::Warning("sup::sequencer::ParseProcedureString('%s') - could not parse string",
                xml_head.c_str());
    return {};
  }

  auto proc = ParseProcedure(*data);

  if (!proc)
  {
    log::Warning("sup::sequencer::ParseProcedureString('%s') - could not parse structure in string!",
                xml_head.c_str());
  }

  return proc;
}

//! Returns XML string representing Procedure.
std::string GetXMLString(const Procedure& procedure)
{
  auto tree_data = ToTreeData(procedure);
  return GetXMLString(*tree_data);
}

}  // namespace sequencer

}  // namespace sup
