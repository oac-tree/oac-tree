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

#include <common/SysTools.h>
#include <common/log-api.h>

// Local header files

#include "UnitTestHelper.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer::LogUI"

// Type definition

namespace sup {

namespace UnitTestHelper {

// Global variables

static const char * ENV_TEST_RESOURCES_PATH_NAME = "TEST_RESOURCES_PATH";

// Function declaration

// Function definition

std::string GetFullTestFilePath(const std::string & filename)
{
  if (filename.empty() || filename[0] == '/')
  {
    return filename;
  }
  std::string resources_path(::ccs::HelperTools::GetEnvironmentVariable(
      ENV_TEST_RESOURCES_PATH_NAME));
  if (resources_path.empty())
  {
    return filename;
  }
  if (resources_path[resources_path.size()-1] != '/')
  {
    resources_path.push_back('/');
  }
  return resources_path + filename;
}

} // namespace UnitTestHelper

} // namespace sup

extern "C" {

// C API function definitions

} // extern C

#undef LOG_ALTERN_SRC
