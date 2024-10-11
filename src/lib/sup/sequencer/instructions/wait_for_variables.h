/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Ricardo Torres (EXT)
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

#ifndef SUP_SEQUENCER_WAIT_FOR_VARIABLES_H_
#define SUP_SEQUENCER_WAIT_FOR_VARIABLES_H_

#include <sup/sequencer/instruction.h>

#include <sup/dto/basic_scalar_types.h>

#include <condition_variable>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction node that waits 'timeout' seconds for all variables of a certain type to be
 * available, i.e. as defined by Variable::IsAvailable().
 *
 * @details The 'timeout' attribute is mandatory.
 */
class WaitForVariables : public Instruction
{
public:
  WaitForVariables();

  ~WaitForVariables() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void RegisterCallbacks(Workspace& ws, std::condition_variable& cv,
                         void* listener, const std::vector<std::string>& var_names) const;

  std::vector<std::string> UnavailableVars(UserInterface& ui, Workspace& ws,
                                           const std::vector<std::string>& var_names) const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_WAIT_FOR_VARIABLES_H_
