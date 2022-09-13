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

// Global header files

#include <common/SysTools.h>  // Misc. helper functions

#include <iostream>  // std::cout, etc.

#include <sup/sequencer/log.h>  // CCS logging library

// Local header files

#include "DaemonInterface.h"
#include <sup/sequencer/runner.h>
#include <sup/sequencer/sequence_parser.h>

// Type definition

struct DaemonParams
{
  bool print_usage;
  char filepath[PATH_MAX_LENGTH];
  bool logging;
  int exit_code;
};

// Global variables

// Function declaration

DaemonParams ParseCommandLineArgs(int argc, char* argv[]);
bool IsHelpOption(const char* option);
bool IsFileOption(const char* option);
bool IsLogOption(const char* option);

// Function definition

void print_usage()
{
  char prog_name[STRING_MAX_LENGTH] = STRING_UNDEFINED;
  ccs::HelperTools::GetProcessName(prog_name);

  std::cout << "Usage: " << prog_name << " <options>" << std::endl;
  std::cout << "Options: -h|--help: Print usage." << std::endl;
  std::cout << "         -f|--file <filename>: Load, parse and execute <filename>." << std::endl;
  std::cout << "         -l|--logging: Enable logging." << std::endl;
  std::cout << std::endl;
  std::cout << "The program loads <filename>, parses it into an executable behaviour tree and "
               "executes it."
            << std::endl;
  std::cout << std::endl;

  return;
}

int main(int argc, char* argv[])
{
  auto params = ParseCommandLineArgs(argc, argv);
  if (params.print_usage)
  {
    print_usage();
    return params.exit_code;
  }

  if (ccs::HelperTools::StringCompare(params.filepath, STRING_UNDEFINED))
  {
    sup::sequencer::log::Warning("sequencer-daemon called without filename");
    return 1;
  }

  sup::sequencer::log::Debug("sequencer-daemon called with filename: %s", params.filepath);

  if (!ccs::HelperTools::Exist(params.filepath))
  {
    sup::sequencer::log::Error("sequencer-daemon: file not found <%s>", params.filepath);
    return 1;
  }

  auto proc = sup::sequencer::ParseProcedureFile(params.filepath);
  if (!proc)
  {
    sup::sequencer::log::Error("sequencer-daemon couldn't parse file <%s>", params.filepath);
    return 1;
  }

  if (!proc->Setup())
  {
    sup::sequencer::log::Error("sequencer-daemon couldn't setup the parsed procedure from file: <%s>",
              params.filepath);
    return 1;
  }

  sup::sequencer::DaemonInterface ui(params.logging);
  sup::sequencer::Runner runner(&ui);
  runner.SetProcedure(proc.get());
  runner.ExecuteProcedure();
  proc->Reset();
  return 0;
}

DaemonParams ParseCommandLineArgs(int argc, char* argv[])
{
  DaemonParams result = {false, STRING_UNDEFINED, false, 0};
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
    else if (IsLogOption(argv[index]))
    {
      // Enable logging of UserInterface methods
      result.logging = true;
    }
    else
    {
      result.print_usage = true;
      result.exit_code = 1;
    }
  }
  return result;
}

bool IsHelpOption(const char* option)
{
  bool result = ccs::HelperTools::StringCompare(option, "-h")
                || ccs::HelperTools::StringCompare(option, "--help");
  return result;
}

bool IsFileOption(const char* option)
{
  bool result = ccs::HelperTools::StringCompare(option, "-f")
                || ccs::HelperTools::StringCompare(option, "--file");
  return result;
}

bool IsLogOption(const char* option)
{
  bool result = ccs::HelperTools::StringCompare(option, "-l")
                || ccs::HelperTools::StringCompare(option, "--logging");
  return result;
}
