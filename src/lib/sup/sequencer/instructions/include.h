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

#ifndef SUP_SEQUENCER_INCLUDE_H_
#define SUP_SEQUENCER_INCLUDE_H_

#include <sup/sequencer/decorator_instruction.h>

namespace sup
{
namespace sequencer
{
/**
 * @brief Decorator that includes an instruction tree by reference.
 *
 * @details The reference can point to an instruction tree in the same definition
 * file or to one defined in a separate file ('file' attribute).
 */
class Include : public DecoratorInstruction
{
public:
  Include();

  ~Include() override;

  static const std::string Type;

private:
  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  bool PostInitialiseVariables(const StringAttributeList& source_attributes) override;

  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus CalculateStatus() const;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INCLUDE_H_
