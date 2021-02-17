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

// Function declaration

// Function definition

bool LoadPlugin(const std::string & name)
{
  bool status = ::ccs::HelperTools::LoadSharedLibrary(name.c_str());

  if (!status)
  {
    log_warning("sup::sequencer::LoadPlugin('%s') - could not load plugin!", name.c_str());
  }

  return status;
}

std::unique_ptr<Procedure> ParseProcedureFile(const std::string & filename)
{
  log_info("sup::sequencer::ParseProcedureFile('%s') - load file..", filename.c_str());
  auto data = ParseXMLData(filename);

  if (!data)
  {
    log_warning("sup::sequencer::ParseProcedureFile('%s') - could not parse file!", filename.c_str());
    return {};
  }

  auto proc = ParseProcedure(*data);

  if (!proc)
  {
    log_warning("sup::sequencer::ParseProcedureFile('%s') - could not parse structure in file!", filename.c_str());
  }

  return proc;
}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
