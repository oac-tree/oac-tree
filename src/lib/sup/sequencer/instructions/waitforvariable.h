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
  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  /**
   * @brief Timeout (in nanoseconds).
   */
  unsigned long m_timeout;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_WAIT_FOR_VARIABLE_H_
