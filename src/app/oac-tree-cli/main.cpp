/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "cl_interface.h"

#include <sup/cli/command_line_parser.h>
#include <sup/log/default_loggers.h>
#include <sup/oac-tree/application_utils.h>
#include <sup/oac-tree/async_runner.h>
#include <sup/oac-tree/job_state_monitor.h>
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/log_severity.h>

#include <exception>
#include <iostream>

using namespace sup::oac_tree;

int main(int argc, char* argv[])
{
  sup::cli::CommandLineParser parser;
  parser.SetDescription(
      /*header*/ "",
      "The program loads <filename>, parses it into an executable behaviour tree and "
      "executes it.");
  parser.AddHelpOption();

  parser.AddOption({"-f", "--file"}, "Load, parse and execute <filename>")
      .SetParameter(true)
      .SetValueName("filename")
      .SetRequired(true);
  parser.AddOption({"-v", "--verbose"}, "Log to standard output (default: WARNING)")
      .SetParameter(true)
      .SetValueName("log_level")
      .SetDefaultValue("WARNING");
  parser.AddOption({"-V", "--validate"}, "Only parse and setup the procedure without running it");

  if (!parser.Parse(argc, argv))
  {
    std::cout << parser.GetUsageString();
    return 0;
  }

  auto filename = parser.GetValue<std::string>("--file");
  auto severity_name = parser.GetValue<std::string>("--verbose");

  auto severity = sup::oac_tree::log::GetSeverityFromString(severity_name);

  auto log_function = [](int severity, const std::string& source, const std::string& message)
                      {
                        std::cout << sup::log::DefaultStdoutLogMessage(severity, source, message)
                                  << std::endl;
                      };

  sup::log::BasicLogger logger(log_function, "oac-tree-cli", severity);
  CLInterface ui(logger);
  utils::SimpleJobStateMonitor monitor;

  std::string error_msg;
  auto proc = utils::SafeParseProcedure(filename, error_msg);
  if (!proc)
  {
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_msg);
    return 1;
  }
  auto async_runner = utils::CreateAsyncRunner(*proc, ui, monitor, error_msg);
  if (!async_runner)
  {
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_msg);
    return 1;
  }

  if (parser.IsSet("--validate"))
  {
    std::cout << "Procedure parsing and setup successful: " << filename << std::endl;
    return 0;  // Early exit.
  }
  async_runner->Start();

  auto end_state = monitor.WaitForFinished();
  std::cout << "Procedure ended with state: " << ToString(end_state) << std::endl;
  return 0;
}
