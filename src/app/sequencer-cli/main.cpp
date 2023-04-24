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
 * Copyright (c) : 2010-2023 ITER Organization,
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
  parser.AddOption({"-v", "--verbose"}, "Log to standard output")
      .SetParameter(true)
      .SetDefaultValue("WARNING");

  if (!parser.Parse(argc, argv))
  {
    std::cout << parser.GetUsageString();
    return 0;
  }

  auto filename = parser.GetValue<std::string>("--file");
  auto severity_name = parser.GetValue<std::string>("--verbose");

  auto severity = sup::sequencer::log::GetSeverityFromString(severity_name);

  sup::log::BasicLogger logger(sup::log::DefaultStdoutLogMessage, "sequencer-cli", severity);
  if (!utils::FileExists(filename))
  {
    std::cout << "main(): Procedure file not found [" + filename + "]" << std::endl;
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
  Runner runner(ui);
  runner.SetProcedure(proc.get());
  runner.ExecuteProcedure();
  proc->Reset();
  return 0;
}
