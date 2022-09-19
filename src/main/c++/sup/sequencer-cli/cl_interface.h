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

#ifndef SUP_SEQUENCER_CL_INTERFACE_H_
#define SUP_SEQUENCER_CL_INTERFACE_H_

#include <sup/sequencer/user_interface.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Class implementing a command line user interface for the Sequencer.
 */
class CLInterface : public UserInterface
{
private:
  /**
   * @brief See sup::sequencer::UserInterface.
   */
  void UpdateInstructionStatusImpl(const Instruction* instruction) override;
  void VariableUpdatedImpl(const std::string& name, const sup::dto::AnyValue& value) override;
  bool PutValueImpl(const sup::dto::AnyValue& value, const std::string& description) override;
  bool GetUserValueImpl(sup::dto::AnyValue& value, const std::string& description) override;
  int GetUserChoiceImpl(const std::vector<std::string>& choices,
                        const std::string& description) override;
  void StartSingleStepImpl() override;
  void EndSingleStepImpl() override;
  void MessageImpl(const std::string& message) override;

  bool _verbose;

protected:
public:
  /**
   * @brief Constructor.
   */
  CLInterface(bool verbose = false);

  /**
   * @brief Destructor.
   */
  ~CLInterface() override;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CL_INTERFACE_H_
