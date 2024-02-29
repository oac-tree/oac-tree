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

#ifndef SUP_SEQUENCER_PROCEDURE_PREAMBLE_H_
#define SUP_SEQUENCER_PROCEDURE_PREAMBLE_H_

#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief Class representing the registration of an AnyType during the procedure's preamble.
 *
 * @note Registration is either done by providing a JSON string or a filename for a file containing
 * the JSON representation of the type.
 */
class TypeRegistrationInfo
{
public:
  enum RegistrationMode
  {
    kJSONFile = 0,
    kJSONString
  };
  TypeRegistrationInfo(RegistrationMode mode, const std::string& str);
  ~TypeRegistrationInfo();

  RegistrationMode GetRegistrationMode() const;
  std::string GetString() const;

private:
  RegistrationMode m_mode;
  std::string m_str;
};

/**
 * @brief Structure containing global information about registered types and loaded plugins. This
 * corresponds to the preamble of its XML representation.
 *
 */
class ProcedurePreamble
{
public:
  ProcedurePreamble();
  ~ProcedurePreamble();

  /**
   * @brief Add a type registration to the preamble.
   *
   * @param info TypeRegistration object.
   */
  void AddTypeRegistration(const TypeRegistrationInfo& info);

  /**
   * @brief Get a list of all type registrations from the preamble.
   */
  std::vector<TypeRegistrationInfo> GetTypeRegistrations() const;

  /**
   * @brief Add a plugin path to the preamble.
   *
   * @param path Path of the plugin.
   */
  void AddPluginPath(const std::string& path);

  /**
   * @brief Get a list of all plugin paths from the preamble.
   */
  std::vector<std::string> GetPluginPaths() const;

private:
  std::vector<TypeRegistrationInfo> m_type_registrations;
  std::vector<std::string> m_plugin_paths;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_PREAMBLE_H_
