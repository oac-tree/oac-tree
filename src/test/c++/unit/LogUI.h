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
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

/**
 * @file LogUI.h
 * @brief Header file for LogUI class.
 * @date 16/12/20
 * @author Walter Van Herck (IO)
 * @copyright 2010-2020 ITER Organization
 * @details This header file contains the definition of the LogUI class.
 */

#ifndef _SEQ_LogUI_h_
#define _SEQ_LogUI_h_

// Global header files

// Local header files

#include "UserInterface.h"

// Constants

// Type definition

#ifdef __cplusplus

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

// Global variables

// Function declarations

// Function definitions

}  // namespace sequencer

}  // namespace sup

extern "C"
{
#endif  // __cplusplus

  // C API function declarations

#ifdef __cplusplus
}  // extern C
#endif  // __cplusplus

#endif  // _SEQ_LogUI_h_
