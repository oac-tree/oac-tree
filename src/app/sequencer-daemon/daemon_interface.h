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

  void UpdateInstructionStatus(const Instruction* instruction) override;
  void VariableUpdated(const std::string& name, const sup::dto::AnyValue& value, bool connected) override;
  bool PutValue(const sup::dto::AnyValue& value, const std::string& description = {}) override;
  bool GetUserValue(sup::dto::AnyValue& value, const std::string& description = {}) override;
  int GetUserChoice(const std::vector<std::string>& options,
                        const sup::dto::AnyValue& metadata = {}) override;
  std::unique_ptr<IUserInputFuture> RequestUserInput(const UserInputRequest& request) override;
  void Message(const std::string& message) override;
  void Log(int severity, const std::string& message) override;

private:
  const sup::log::DefaultLogger m_logger;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_DAEMON_INTERFACE_H_
