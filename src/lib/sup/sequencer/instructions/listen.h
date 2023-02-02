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

#ifndef SUP_SEQUENCER_LISTEN_H_
#define SUP_SEQUENCER_LISTEN_H_

#include "decorator_instruction.h"

#include <sup/sequencer/workspace.h>

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator that executes its child instruction each time specific variables are updated.
 */
class Listen : public DecoratorInstruction
{
public:
  Listen();

  ~Listen() override;

  static const std::string Type;

private:
  using CBGuard = CallbackGuard<NamedCallbackManager<const sup::dto::AnyValue&>>;

  bool force_success;
  bool var_changed;
  std::mutex mx;
  std::condition_variable cv;
  std::vector<std::string> var_names;
  std::map<std::string, sup::dto::AnyValue> var_cache;
  CBGuard cb_guard;

  /**
   * @brief Repeatedly execute the child instruction when specific variables are updated.
   * When forceSuccess is not set, the instruction only ends on the child's FAILURE and returns
   * FAILURE accordingly.
   * The instruction also immediately returns SUCCESS in the absence of a child instruction.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface* ui, Workspace* ws) override;

  void HaltImpl() override;
  void SetupImpl(const Procedure& proc) override;

  /**
   * @brief Calculate this instruction's status from the status of its child instruction.
   */
  ExecutionStatus CalculateStatus() const;

  std::vector<std::string> VariableNames() const;

  void UpdateCallback(const std::string& name, const sup::dto::AnyValue& val);

  void RegisterCallbacks(Workspace* ws, std::vector<std::string> var_names);
  void ClearCallbacks();
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LISTEN_H_
