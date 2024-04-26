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
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sup/sequencer/application_utils.h>
#include <sup/sequencer/job_controller.h>
#include <sup/sequencer/job_state_monitor.h>
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/log_severity.h>

#include <exception>
#include <iostream>

using namespace sup::sequencer;

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

  auto severity = sup::sequencer::log::GetSeverityFromString(severity_name);

  auto log_function = [](int severity, const std::string& source, const std::string& message)
                      {
                        std::cout << sup::log::DefaultStdoutLogMessage(severity, source, message)
                                  << std::endl;
                      };

  sup::log::BasicLogger logger(log_function, "sequencer-cli", severity);
  CLInterface ui(logger);
  utils::SimpleJobStateMonitor monitor;

  std::string error_msg;
  auto proc = utils::SafeParseProcedure(filename, error_msg);
  if (!proc)
  {
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_msg);
    return 1;
  }
  auto job_controller = utils::CreateJobController(*proc, ui, monitor, error_msg);
  if (!job_controller)
  {
    logger.LogMessage(log::SUP_SEQ_LOG_ERR, error_msg);
    return 1;
  }

  if (parser.IsSet("--validate"))
  {
    std::cout << "Procedure parsing and setup successful: " << filename << std::endl;
    return 0;  // Early exit.
  }
  job_controller->Start();

  auto end_state = monitor.WaitForFinished();
  std::cout << "Procedure ended with state: " << ToString(end_state) << std::endl;
  return 0;
}
