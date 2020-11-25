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

#include <iostream> // std::cout, etc.

#include <common/log-api.h> // CCS logging library
#include <common/SysTools.h> // Misc. helper functions

// Local header files

#include "parser/SequenceParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

// Global variables

// Function declaration

// Function definition

void print_usage()
{
  char prog_name[STRING_MAX_LENGTH] = STRING_UNDEFINED;
  ccs::HelperTools::GetProcessName(prog_name);

  std::cout << "Usage: " << prog_name << " <options>" << std::endl;
  std::cout << "Options: -h|--help: Print usage." << std::endl;
  std::cout << "         -f|--file <filename>: Load, parse and execute <filename>." << std::endl;
  std::cout << "         -v|--verbose: Log to standard output." << std::endl;
  std::cout << std::endl;
  std::cout << "The program loads <filename>, parses it into an executable behaviour tree and executes it." << std::endl;
  std::cout << std::endl;

  return;
}

int main(int argc, char * argv[])
{
  char filepath[PATH_MAX_LENGTH] = STRING_UNDEFINED;

  if (argc > 1)
  {
    for (unsigned index = 1; index < (unsigned)argc; index++)
    {
      if (ccs::HelperTools::StringCompare(argv[index], "-h")
          || ccs::HelperTools::StringCompare(argv[index], "--help"))
      {
        // Display usage
        print_usage();
        return 0;
      }
      else if (ccs::HelperTools::StringCompare(argv[index], "-f")
               || ccs::HelperTools::StringCompare(argv[index], "--file"))
      {
        // Get filename
        if (index + 1 >= (unsigned)argc)
        {
          print_usage();
          return 1;
        }
        ccs::HelperTools::SafeStringCopy(filepath, argv[index+1], PATH_MAX_LENGTH);
        index += 1;
      }
      else if (ccs::HelperTools::StringCompare(argv[index], "-v")
               || ccs::HelperTools::StringCompare(argv[index], "--verbose"))
      {
        // Log to standard output
        (void)ccs::log::SetStdout();
        (void)ccs::log::SetFilter(LOG_DEBUG);
      }
      else
      {
        print_usage();
        return 1;
      }
    }
  }
  else
  {
    print_usage();
    return 0;
  }
  log_info("sequencer-cli called with filename: %s", filepath);
  return 0;
}

#undef LOG_ALTERN_SRC
