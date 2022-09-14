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

#include <common/AnyValue.h>
#include <common/BasicTypes.h>

namespace sup
{
namespace sequencer
{
class Choice : public CompoundInstruction
{
private:
  /**
   * @brief Executes a subset of children depending on a variable value
   *
   * @return SUCCESS if all the selected children return SUCCESS.
   */
  ExecutionStatus ExecuteSingleImpl(UserInterface *ui, Workspace *ws) override;

  /**
   * @brief child execution
   */
  ExecutionStatus ExecuteChild(::ccs::types::uint32 idx, UserInterface *ui, Workspace *ws);

  bool CheckIfSelectorArray(const ::ccs::types::AnyValue &_val);

  bool CheckSelectorType(const Procedure &proc);

  ExecutionStatus ExecuteBitChild(const ::ccs::types::uint64 value,
                                  const ::ccs::types::uint32 remained, UserInterface *ui,
                                  Workspace *ws);

  ExecutionStatus ExecuteMaskSelector(::ccs::types::uint8 *valPtr, UserInterface *ui,
                                      Workspace *ws);

  ExecutionStatus ExecuteArraySelector(::ccs::types::uint8 *valPtr, UserInterface *ui,
                                       Workspace *ws);

  bool SetupImpl(const Procedure &proc) override;

  std::string varName;

  ::ccs::types::uint32 numberOfElements;

  ::ccs::types::uint32 elementSize;

  bool isMask;

public:
  Choice();

  virtual ~Choice();

  static const std::string Type;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_CHOICE_H_
