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

#ifndef SUP_SEQUENCER_LOG_UI_H_
#define SUP_SEQUENCER_LOG_UI_H_

#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Class implementing a log interface for the Sequencer.
 */
class LogUI : public UserInterface
{
private:
  /**
   * @brief See sup::sequencer::UserInterface.
   */
  void UpdateInstructionStatusImpl(const Instruction* instruction) override;
  void StartSingleStepImpl() override;
  void EndSingleStepImpl() override;

protected:
public:
  /**
   * @brief Constructor.
   */
  LogUI();

  /**
   * @brief Destructor.
   */
  ~LogUI() override;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LOG_UI_H_
