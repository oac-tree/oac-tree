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

#include <sup/sequencer/procedure_preamble.h>

namespace sup
{
namespace sequencer
{

TypeRegistrationInfo::TypeRegistrationInfo(RegistrationMode mode, const std::string& str)
  : m_mode{mode}
  , m_str{str}
{}

TypeRegistrationInfo::~TypeRegistrationInfo() = default;

TypeRegistrationInfo::RegistrationMode TypeRegistrationInfo::GetRegistrationMode() const
{
  return m_mode;
}

std::string TypeRegistrationInfo::GetString() const
{
  return m_str;
}

ProcedurePreamble::ProcedurePreamble()
  : m_type_registrations{}
  , m_plugin_paths{}
{}

ProcedurePreamble::~ProcedurePreamble() = default;

void ProcedurePreamble::AddTypeRegistration(const TypeRegistrationInfo& info)
{
  m_type_registrations.push_back(info);
}

std::vector<TypeRegistrationInfo> ProcedurePreamble::GetTypeRegistrations() const
{
  return m_type_registrations;
}

void ProcedurePreamble::AddPluginPath(const std::string& path)
{
  m_plugin_paths.push_back(path);
}

std::vector<std::string> ProcedurePreamble::GetPluginPaths() const
{
  return m_plugin_paths;
}

}  // namespace sequencer

}  // namespace sup
