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

/**
 * @file DaemonInterface.h
 * @brief Header file for DaemonInterface class.
 * @date 05/05/21
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the DaemonInterface class.
 */

#ifndef _SEQ_DaemonInterface_h_
#define _SEQ_DaemonInterface_h_

// Global header files

// Local header files

#include <sup/sequencer/user_interface.h>

// Constants

// Type definition

#ifdef __cplusplus

namespace sup
{
namespace sequencer
{
/**
 * @brief Class implementing a write-only user interface for the Sequencer daemon procedures.
 */

class DaemonInterface : public UserInterface
{
private:
  /**
   * @brief See sup::sequencer::UserInterface.
   */
  void UpdateInstructionStatusImpl(const Instruction* instruction) override;
  bool GetUserValueImpl(::ccs::types::AnyValue& value, const std::string& description) override;
  int GetUserChoiceImpl(const std::vector<std::string>& choices,
                        const std::string& description) override;
  void StartSingleStepImpl() override;
  void EndSingleStepImpl() override;
  void MessageImpl(const std::string& message) override;

  bool _log_enabled;

protected:
public:
  /**
   * @brief Constructor.
   */
  DaemonInterface(bool log_enabled = false);

  /**
   * @brief Destructor.
   */
  ~DaemonInterface() override;
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

#endif  // _SEQ_DaemonInterface_h_
