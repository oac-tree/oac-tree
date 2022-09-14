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

#include "choice.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/workspace.h>

#include <common/AnyType.h>

namespace sup
{
namespace sequencer
{

const std::string Choice::Type = "Choice";

Choice::Choice()
  : CompoundInstruction(Choice::Type)
  , numberOfElements{1u}
  , elementSize{0}
  , isMask{false}
{}

Choice::~Choice()
{}

bool Choice::CheckIfSelectorArray(const ::ccs::types::AnyValue &_val)
{
  // check if array
  bool ret = true;
  if (!isMask)
  {
    ::ccs::base::SharedReference<const ::ccs::types::ArrayType> myArrayType = _val.GetType();
    if (myArrayType.IsValid())
    {
      numberOfElements = myArrayType->GetElementNumber();
      elementSize = myArrayType->GetElementType()->GetSize();
    }
    // they must be arrays of uint32 or positive int32
    ret = (elementSize <= 4u);
    if (!ret)
    {
      log::Error("Choice::Setup - element size must be <= 4");
    }
  }
  return ret;
}

bool Choice::CheckSelectorType(const Procedure &proc)
{
  varName = GetAttribute("var_name");
  ::ccs::types::AnyValue _val;
  bool ret = proc.GetVariableValue(varName, _val);
  if (ret)
  {
    elementSize = _val.GetSize();
    isMask = HasAttribute("is_mask");
    if (isMask)
    {
      isMask = (GetAttribute("is_mask") == "true");
    }
    ret = CheckIfSelectorArray(_val);
  }
  return ret;
}

bool Choice::SetupImpl(const Procedure &proc)
{
  bool ret = SetupChildren(proc);
  if (ret)
  {
    ret = HasAttribute("var_name");
    if (ret)
    {
      ret = CheckSelectorType(proc);
    }
    else
    {
      log::Error("Choice::Setup - No attribute var_name found");
    }

    log::Debug("Choice::Setup - With var_name=%s numberOfElements=%u elementSize=%u isMask=%u",
              varName.c_str(), numberOfElements, elementSize, isMask);
  }
  return ret;
}

ExecutionStatus Choice::ExecuteBitChild(const ::ccs::types::uint64 value,
                                        const ::ccs::types::uint32 remained, UserInterface *ui,
                                        Workspace *ws)
{
  ExecutionStatus child_status = ExecutionStatus::SUCCESS;

  bool exit = false;
  for (::ccs::types::uint32 i = 0u; (i < remained) && (!exit); i++)
  {
    log::Debug("Choice::ExecuteSingleImpl - Considering bit %d of %d", i, remained);
    if (((value >> i) & (0x1u)))
    {
      child_status = ExecuteChild(i, ui, ws);
      // continue only if success
      exit = (child_status != ExecutionStatus::SUCCESS);
      // child_status = _children[i]->GetStatus();
    }
  }
  return child_status;
}

ExecutionStatus Choice::ExecuteMaskSelector(::ccs::types::uint8 *valPtr, UserInterface *ui,
                                            Workspace *ws)
{
  ExecutionStatus child_status = ExecutionStatus::SUCCESS;

  ::ccs::types::uint32 nElems = (elementSize * sizeof(::ccs::types::uint64));
  log::Debug("Choice::ExecuteSingleImpl - isMask nElems=%d", nElems);
  bool exit = false;
  // generic...can consider very big elements
  while ((nElems > 0u) && (!exit))
  {
    ::ccs::types::uint32 remained = (nElems > 64u) ? (64u) : (nElems);
    ::ccs::types::uint64 value = 0u;
    memcpy(&value, valPtr, (remained / 8u));
    log::Debug("Choice::ExecuteSingleImpl - isMask value=%d", value);
    child_status = ExecuteBitChild(value, remained, ui, ws);
    exit = (child_status != ExecutionStatus::SUCCESS);
    nElems -= remained;
    valPtr += sizeof(::ccs::types::uint64);
  }
  return child_status;
}

ExecutionStatus Choice::ExecuteArraySelector(::ccs::types::uint8 *valPtr, UserInterface *ui,
                                             Workspace *ws)
{
  ExecutionStatus child_status = ExecutionStatus::SUCCESS;

  for (::ccs::types::uint32 i = 0u; i < numberOfElements; i++)
  {
    ::ccs::types::uint32 value = 0u;

    memcpy(&value, valPtr, elementSize);

    child_status = ExecuteChild(value, ui, ws);
    // continue only if success
    if (child_status != ExecutionStatus::SUCCESS)
    {
      // child_status = _children[value]->GetStatus();
      break;
    }
    valPtr += elementSize;
  }
  return child_status;
}

ExecutionStatus Choice::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
  ExecutionStatus child_status = ExecutionStatus::SUCCESS;
  ::ccs::types::AnyValue _val;
  ws->GetValue(varName, _val);
  auto valPtr = reinterpret_cast<::ccs::types::uint8 *>(_val.GetInstance());

  if (isMask)
  {
    child_status = ExecuteMaskSelector(valPtr, ui, ws);
  }
  else
  {
    child_status = ExecuteArraySelector(valPtr, ui, ws);
  }
  return child_status;
}

ExecutionStatus Choice::ExecuteChild(::ccs::types::uint32 idx, UserInterface *ui, Workspace *ws)
{
  ExecutionStatus child_status = ExecutionStatus::SUCCESS;
  if (idx < ChildInstructions().size())
  {
    child_status = ChildInstructions()[idx]->GetStatus();

    if (NeedsExecute(child_status))
    {
      auto childName = ChildInstructions()[idx]->GetName();
      log::Debug("Choice::ExecuteChild - Executing child[%u]=%s", idx, childName.c_str());

      ChildInstructions()[idx]->ExecuteSingle(ui, ws);
      child_status = ExecutionStatus::NOT_FINISHED;
    }
  }
  else
  {
    log::Warning(
        "Status Choice::ExecuteSingleImpl - child[%u] not executed because exceeding children size "
        "(%u)",
        idx, ChildInstructions().size());
  }
  return child_status;
}

}  // namespace sequencer

}  // namespace sup
