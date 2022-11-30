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

#include "daemon_interface.h"

#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/log.h>
#include <sup/sequencer/runner.h>
#include <sup/sequencer/sequence_parser.h>

#include <iostream>
#include <string>
#include <string.h>

struct DaemonParams
{
  bool print_usage;
  std::string filepath;
  bool logging;
  int exit_code;
};

static const std::string kProgramName = "sequencer-daemon";

DaemonParams ParseCommandLineArgs(int argc, char* argv[]);
bool IsHelpOption(const char* option);
bool IsFileOption(const char* option);
bool IsLogOption(const char* option);
bool IsEqualCString(const char* lhs, const char* rhs);

void print_usage()
{
  std::cout << "Usage: " << kProgramName << " <options>" << std::endl;
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

  if (params.filepath.empty())
  {
    sup::sequencer::log::Warning("sequencer-daemon called without filename");
    return 1;
  }
  if (!sup::sequencer::utils::FileExists(params.filepath))
  {
    sup::sequencer::log::Error("sequencer-daemon: file not found <%s>", params.filepath.c_str());
    return 1;
  }

  auto proc = sup::sequencer::ParseProcedureFile(params.filepath);
  if (!proc)
  {
    sup::sequencer::log::Error("sequencer-daemon couldn't parse file <%s>", params.filepath.c_str());
    return 1;
  }

  if (!proc->Setup())
  {
    sup::sequencer::log::Error("sequencer-daemon couldn't setup the parsed procedure from file: <%s>",
              params.filepath.c_str());
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
  DaemonParams result = {false, "", false, 0};
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
      else
      {
        result.filepath = argv[++index];
      }
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
  bool result = IsEqualCString(option, "-h") || IsEqualCString(option, "--help");
  return result;
}

bool IsFileOption(const char* option)
{
  bool result = IsEqualCString(option, "-f") || IsEqualCString(option, "--file");
  return result;
}

bool IsLogOption(const char* option)
{
  bool result = IsEqualCString(option, "-l") || IsEqualCString(option, "--logging");
  return result;
}

bool IsEqualCString(const char* lhs, const char* rhs)
{
  return strcmp(lhs, rhs) == 0;
}
