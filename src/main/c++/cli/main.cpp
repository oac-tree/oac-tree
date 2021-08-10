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

#include <algorithm>
#include <iostream>  // std::cout, etc.

#include <common/log-api.h>  // CCS logging library

// Local header files

#include "CLInterface.h"
#include "Runner.h"
#include "SequenceParser.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

namespace
{
enum VerbosityLevels
{
  kSilent,
  kMinimal,
  kInfo
};
}  // namespace

// Type definition

// Global variables

// Function declaration

bool IsVerboseOption(const char* option);
bool HasHelpOption(const std::vector<std::string>& arguments);
std::string GetFileName(const std::vector<std::string>& arguments);
int GetVerbosityLevel(const std::vector<std::string>& arguments);

// Function definition

void print_usage(const std::string& prog_name)
{
  std::cout << "Usage: " << prog_name << " <options>" << std::endl;
  std::cout << "Options: -h|--help: Print usage." << std::endl;
  std::cout << "         -f|--file <filename>: Load, parse and execute <filename>." << std::endl;
  std::cout << "         -v|--verbose: Log to standard output." << std::endl;
  std::cout << "         -vv : Maximum logging verbosity" << std::endl;
  std::cout << std::endl;
  std::cout << "The program loads <filename>, parses it into an executable behaviour tree and "
               "executes it."
            << std::endl;
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  std::vector<std::string> arguments;
  std::for_each(argv, argv + argc, [&](const char* c_str) { arguments.push_back(c_str); });

  auto filename = GetFileName(arguments);

  if (HasHelpOption(arguments) || filename.empty())
  {
    print_usage(arguments.at(0));
    return 0;
  }
  log_debug("sequencer-cli called with filename: %s", filename.c_str());

  if (!ccs::HelperTools::Exist(filename.c_str()))
  {
    log_error("sequencer-cli: file not found <%s>", filename.c_str());
    return 1;
  }

  auto verbosity = GetVerbosityLevel(arguments);
  if (verbosity > 0)
  {
    (void)ccs::log::SetStdout();
    (void)ccs::log::SetFilter(verbosity == kMinimal ? LOG_NOTICE : LOG_DEBUG);
  }

  auto proc = sup::sequencer::ParseProcedureFile(filename.c_str());
  if (!proc)
  {
    log_error("sequencer-cli couldn't parse file <%s>", filename.c_str());
    return 1;
  }

  if (!proc->Setup())
  {
    log_error("sequencer-cli couldn't setup the parsed procedure from file: <%s>",
              filename.c_str());
    return 1;
  }

  sup::sequencer::CLInterface ui(verbosity > 0);
  sup::sequencer::Runner runner(&ui);
  runner.SetProcedure(proc.get());
  runner.ExecuteProcedure();
  proc->Reset();
  return 0;
}

//! Returns true if -h or --help option is present.

bool HasHelpOption(const std::vector<std::string>& arguments)
{
  auto on_argument = [](const std::string& str) { return str == "--help" || str == "-h"; };
  return std::find_if(arguments.begin(), arguments.end(), on_argument) != arguments.end();
}

//! Returns filename, which is the parameter after --file or -f option.

std::string GetFileName(const std::vector<std::string>& arguments)
{
  // find position of file argument
  auto on_argument = [](const std::string& str) { return str == "--file" || str == "-f"; };
  auto it = std::find_if(arguments.begin(), arguments.end(), on_argument);

  // taking the next after as a file name
  std::string filename = std::next(it) < arguments.end() ? *std::next(it) : "";
  return filename.find_first_of("-") == 0 ? "" : filename;
}

//! Returns requested verbosity level.

int GetVerbosityLevel(const std::vector<std::string>& arguments)
{
  static std::map<std::string, int> verbosity_map = {
      {"-v", kMinimal}, {"--verbose", kMinimal}, {"-vv", kInfo}};

  // find position of file argument
  auto on_argument = [](const std::string& str)
  { return str == "-v" || str == "--verbose" || str == "-vv"; };
  auto it = std::find_if(arguments.begin(), arguments.end(), on_argument);

  int result = it < arguments.end() ? verbosity_map[*it] : kSilent;
  return result;
}

#undef LOG_ALTERN_SRC
