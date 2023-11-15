/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) Sequencer component
 *
 * Description   : File-based variable implementation
 *
 * Author        : Bertrand Bauvir (IO)
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

#ifndef SUP_SEQUENCER_FILE_VARIABLE_H_
#define SUP_SEQUENCER_FILE_VARIABLE_H_

#include <sup/sequencer/variable.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief FileVariable class.
 * @details Variable with file-based backend.
 */
class FileVariable : public Variable
{
public:
  FileVariable();
  ~FileVariable() override;

  /**
   * @brief Class name for VariableRegistry.
   */
  static const std::string Type;

private:
  /**
   * @brief See sup::sequencer::Variable.
   */
  bool GetValueImpl(sup::dto::AnyValue& value) const override;
  bool SetValueImpl(const sup::dto::AnyValue& value) override;
  bool IsAvailableImpl() const override;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_FILE_VARIABLE_H_