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

#ifndef SUP_SEQUENCER_LESSTHAN_H_
#define SUP_SEQUENCER_LESSTHAN_H_

#include <sup/sequencer/instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Instruction checking the if a variable is smaller then the other
 *
 * @details Check if variable identified with 'lhs' attribute is smaller than one with 'rhs'
 * attribute.
 */
class LessThan : public Instruction
{
public:
  LessThan();

  ~LessThan() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_LESSTHAN_H_
