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

#ifndef SUP_SEQUENCER_CHOICE_H_
#define SUP_SEQUENCER_CHOICE_H_

#include "compound_instruction.h"

#include <sup/dto/anyvalue.h>

#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief Executes a subset of children depending on a variable value
 *
 * @note The variable either needs to encode a single index for the child to be executed
 * (i.e. a scalar positive integer) or an array of such indices.
 */
class Choice : public CompoundInstruction
{
public:
  Choice();

  virtual ~Choice();

  static const std::string Type;

private:
  std::string m_var_name;

  bool SetupImpl(const Procedure &proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface *ui, Workspace *ws) override;

  ExecutionStatus ExecuteArraySelector(std::vector<std::size_t> indices, UserInterface *ui,
                                       Workspace *ws);

  ExecutionStatus ExecuteChild(std::size_t idx, UserInterface *ui, Workspace *ws);
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CHOICE_H_
