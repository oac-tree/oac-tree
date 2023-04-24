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

#ifndef SUP_SEQUENCER_DAEMON_INTERFACE_H_
#define SUP_SEQUENCER_DAEMON_INTERFACE_H_

#include <sup/sequencer/user_interface.h>

#include <sup/log/default_loggers.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Class implementing a write-only user interface for the Sequencer daemon procedures.
 */
class DaemonInterface : public UserInterface
{
public:
  DaemonInterface(const sup::log::DefaultLogger& logger);

  ~DaemonInterface() override;

private:
  void UpdateInstructionStatusImpl(const Instruction* instruction) override;
  bool PutValueImpl(const sup::dto::AnyValue& value, const std::string& description) override;
  bool GetUserValueImpl(sup::dto::AnyValue& value, const std::string& description) override;
  int GetUserChoiceImpl(const std::vector<std::string>& choices,
                        const std::string& description) override;
  void StartSingleStepImpl() override;
  void EndSingleStepImpl() override;
  void MessageImpl(const std::string& message) override;
  void LogImpl(int severity, const std::string& message) override;

  sup::log::DefaultLogger m_logger;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_DAEMON_INTERFACE_H_
