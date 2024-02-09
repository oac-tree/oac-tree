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

#include <sup/sequencer/application_utils.h>

#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/sequence_parser.h>

namespace sup
{
namespace sequencer
{

namespace utils
{

ProcedureLoader::ProcedureLoader(const std::string& filename)
  : m_filename{filename}
  , m_error_message{}
{}

ProcedureLoader::~ProcedureLoader() = default;

std::unique_ptr<Procedure> ProcedureLoader::ParseAndSetup()
{
  m_error_message.clear();
  if (!utils::FileExists(m_filename))
  {
    m_error_message = "Procedure file not found [" + m_filename + "]";
    return {};
  }
  try
  {
    auto proc = ParseProcedureFile(m_filename);
    if (!proc)
    {
      m_error_message = "Could not parse procedure file [" + m_filename + "]";
      return {};
    }
    proc->Setup();
    return proc;
  }
  catch(const std::exception& e)
  {
    m_error_message = e.what();
  }
  return {};
}

std::string ProcedureLoader::GetErrorMessage() const
{
  return m_error_message;
}

}  // namespace utils

}  // namespace sequencer

}  // namespace sup
