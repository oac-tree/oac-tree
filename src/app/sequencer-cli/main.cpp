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

#include "cl_interface.h"

#include <sup/cli/command_line_parser.h>
#include <sup/log/default_loggers.h>
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/log_severity.h>
#include <sup/sequencer/runner.h>
#include <sup/sequencer/sequence_parser.h>

#include <algorithm>
#include <iostream>

using namespace sup::sequencer;

bool IsVerboseOption(const char* option);
bool HasHelpOption(const std::vector<std::string>& arguments);
std::string GetFileName(const std::vector<std::string>& arguments);
int GetSeverityLevel(const std::vector<std::string>& arguments);

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
  sup::cli::CommandLineParser parser;
  parser.AddHelpOption();
  parser.AddOption({"-f", "--file"}, "Load, parse and execute <filename>")
      .SetParameter(true)
      .SetValueName("filename")
      .SetRequired(true);
  parser.AddOption({"-v", "--verbose"}, "Log to standard output")
      .SetParameter(true)
      .SetDefaultValue("WARNING");

  if (!parser.Parse(argc, argv))
  {
    std::cout << parser.GetUsageString();
    return 0;
  }

  std::vector<std::string> arguments;
  std::for_each(argv, argv + argc, [&](const char* c_str) { arguments.push_back(c_str); });

  auto filename = GetFileName(arguments);

  if (HasHelpOption(arguments) || filename.empty())
  {
    print_usage(arguments.at(0));
    return 0;
  }

  auto severity = GetSeverityLevel(arguments);

  sup::log::BasicLogger logger(sup::log::DefaultStdoutLogMessage, "sequencer-cli", severity);
  if (!utils::FileExists(filename))
  {
    std::string error_message = "main(): Procedure file not found [" + filename + "]";
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return 1;
  }

  auto proc = ParseProcedureFile(filename);
  if (!proc)
  {
    std::string error_message = "main(): could not parse procedure file [" + filename + "]";
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return 1;
  }

  if (!proc->Setup())
  {
    std::string error_message = "main(): could not setup procedure from file [" + filename + "]";
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_message);
    return 1;
  }

  CLInterface ui(logger);
  Runner runner(&ui);
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

//! Returns requested severity level.

int GetSeverityLevel(const std::vector<std::string>& arguments)
{
  static std::map<std::string, int> verbosity_map = {{"-v", log::SUP_SEQ_LOG_WARNING},
                                                     {"--verbose", log::SUP_SEQ_LOG_WARNING},
                                                     {"-vv", log::SUP_SEQ_LOG_INFO}};

  // find position of file argument
  auto on_argument = [](const std::string& str)
  { return str == "-v" || str == "--verbose" || str == "-vv"; };
  auto it = std::find_if(arguments.begin(), arguments.end(), on_argument);

  int result = (it == arguments.end()) ? log::SUP_SEQ_LOG_ERR : verbosity_map[*it];
  return result;
}
