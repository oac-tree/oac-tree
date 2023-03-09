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

#ifndef SUP_SEQUENCER_WAIT_H_
#define SUP_SEQUENCER_WAIT_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction node that returns SUCCESS after a given timeout.
 *
 * @details The 'timeout' attribute is optional. When this attribute is not present,
 * the instruction returns SUCCESS immediately.
 */
class Wait : public Instruction
{
public:
  Wait();

  ~Wait() override;

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

#endif  // SUP_SEQUENCER_WAIT_H_
