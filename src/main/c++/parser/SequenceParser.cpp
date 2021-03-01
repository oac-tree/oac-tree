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
#include <common/SysTools.h>

// Local header files

#include "SequenceParser.h"
#include "SequenceParserImpl.h"
#include "ProcedureParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

// Function definition

bool LoadPlugin(const std::string & name)
{
  log_info("sup::sequencer::LoadPlugin('%s') - trying to load plugin..", name.c_str());
  bool status = ::ccs::HelperTools::LoadSharedLibrary(name.c_str());

  if (!status)
  {
    log_warning("sup::sequencer::LoadPlugin('%s') - could not load plugin", name.c_str());
  }
  else
  {
    log_info("sup::sequencer::LoadPlugin('%s') - successfully loaded plugin", name.c_str());
  }
  return status;
}

std::unique_ptr<Procedure> ParseProcedureFile(const std::string & filename)
{
  log_info("sup::sequencer::ParseProcedureFile('%s') - load file..", filename.c_str());
  auto data = ParseXMLDataFile(filename);

  if (!data)
  {
    log_warning("sup::sequencer::ParseProcedureFile('%s') - could not parse file", filename.c_str());
    return {};
  }

  auto proc = ParseProcedure(*data);

  if (!proc)
  {
    log_warning("sup::sequencer::ParseProcedureFile('%s') - could not parse structure in file!", filename.c_str());
  }

  return proc;
}

std::unique_ptr<Procedure> ParseProcedureString(const std::string & xml_str)
{
  auto xml_head = xml_str.substr(0, 1024);
  log_info("sup::sequencer::ParseProcedureString('%s') - parsing string..", xml_head.c_str());
  auto data = ParseXMLDataString(xml_str);

  if (!data)
  {
    log_warning("sup::sequencer::ParseProcedureString('%s') - could not parse string", xml_head.c_str());
    return {};
  }

  auto proc = ParseProcedure(*data);

  if (!proc)
  {
    log_warning("sup::sequencer::ParseProcedureString('%s') - could not parse structure in string!", xml_head.c_str());
  }

  return proc;
}


} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
