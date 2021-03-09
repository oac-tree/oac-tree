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
#include <memory> // std::unique_ptr
#include <string> // std::string

#include <common/log-api.h> // CCS logging library
#include <common/SysTools.h> // Misc. helper functions

// Local header files

#include "SequenceParser.h"
#include "Runner.h"
#include "CLInterface.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

struct CLIParams
{
  bool print_usage;
  char filepath[PATH_MAX_LENGTH];
  bool verbose;
  int exit_code;
};

// Global variables

// Function declaration

CLIParams ParseCommandLineArgs(int argc, char * argv[]);
bool IsHelpOption(const char * option);
bool IsFileOption(const char * option);
bool IsVerboseOption(const char * option);

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
  auto params = ParseCommandLineArgs(argc, argv);
  if (params.print_usage)
  {
    print_usage();
    return params.exit_code;
  }

  if (ccs::HelperTools::StringCompare(params.filepath, STRING_UNDEFINED))
  {
    log_warning("sequencer-cli called without filename");
    return 1;
  }

  log_info("sequencer-cli called with filename: %s", params.filepath);

  if (!ccs::HelperTools::Exist(params.filepath))
  {
    log_error("sequencer-cli: file not found <%s>", params.filepath);
    return 1;
  }

  auto proc = sup::sequencer::ParseProcedureFile(params.filepath);
  if (!proc)
  {
    log_error("sequencer-cli couldn't parse file <%s>", params.filepath);
    return 1;
  }

  sup::sequencer::CLInterface ui(params.verbose);
  sup::sequencer::Runner runner(&ui);
  runner.SetProcedure(proc.get());
  runner.ExecuteProcedure();
  return 0;
}

CLIParams ParseCommandLineArgs(int argc, char * argv[])
{
  CLIParams result = {false, STRING_UNDEFINED, false, 0};
  if (argc <= 1)
  {
    result.print_usage = true;
    return result;
  }
  for (unsigned index = 1; index < (unsigned)argc; index++)
  {
    if (IsHelpOption(argv[index]))
    {
      result.print_usage = true;
    }
    else if (IsFileOption(argv[index]))
    {
      // Get filename
      if (index + 1 >= (unsigned)argc)
      {
        result.print_usage = true;
        result.exit_code = 1;
      }
      ccs::HelperTools::SafeStringCopy(result.filepath, argv[index + 1], PATH_MAX_LENGTH);
      index += 1;
    }
    else if (IsVerboseOption(argv[index]))
    {
      // Log to standard output
      (void)ccs::log::SetStdout();
      (void)ccs::log::SetFilter(LOG_DEBUG);
      result.verbose = true;
    }
    else
    {
      result.print_usage = true;
      result.exit_code = 1;
    }
  }
  return result;
}

bool IsHelpOption(const char * option)
{
  bool result = ccs::HelperTools::StringCompare(option, "-h")
             || ccs::HelperTools::StringCompare(option, "--help");
  return result;
}

bool IsFileOption(const char * option)
{
  bool result = ccs::HelperTools::StringCompare(option, "-f")
             || ccs::HelperTools::StringCompare(option, "--file");
  return result;
}

bool IsVerboseOption(const char * option)
{
  bool result = ccs::HelperTools::StringCompare(option, "-v")
             || ccs::HelperTools::StringCompare(option, "--verbose");
  return result;
}

#undef LOG_ALTERN_SRC
