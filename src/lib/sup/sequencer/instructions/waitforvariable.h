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

#ifndef SUP_SEQUENCER_WAIT_FOR_VARIABLE_H_
#define SUP_SEQUENCER_WAIT_FOR_VARIABLE_H_

#include <sup/sequencer/instruction.h>

#include <sup/dto/basic_scalar_types.h>

#include <condition_variable>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction node that waits 'timeout' seconds for a variable to be read.
 *
 * @details The 'timeout' attribute is mandatory. There is also the 'equalsVar' option, that
 * will wait that the read variable is equal to the one provided in that variable.
 */
class WaitForVariable : public Instruction
{
public:
  WaitForVariable();

  ~WaitForVariable() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  void RegisterCallback(Workspace& ws, std::condition_variable& cv,
                        void* listener, const std::string& var_name) const;

  bool SuccessCondition(bool var_available, const sup::dto::AnyValue& var_value,
                        bool other_available, const sup::dto::AnyValue& other_value) const;

  bool CheckCondition(Workspace& ws) const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_WAIT_FOR_VARIABLE_H_
