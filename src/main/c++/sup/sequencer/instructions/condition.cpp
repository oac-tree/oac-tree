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

#include "condition.h"

#include <sup/sequencer/log.h>
#include <sup/sequencer/workspace.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <common/BasicTypes.h>

namespace sup
{
namespace sequencer
{
const std::string Condition::Type = "Condition";

Condition::Condition() : Instruction(Condition::Type) {}

Condition::~Condition() = default;

ExecutionStatus Condition::ExecuteSingleImpl(UserInterface * /*ui*/, Workspace *ws)
{
  sup::dto::AnyValue var;
  std::string varName = GetAttribute("var_name");
  bool ret = ws->GetValue(varName, var);

  if (ret)
  {
    ::ccs::base::SharedReference<const sup::dto::ScalarType> varType = var.GetType();
    ret = varType.IsValid();
    if (ret)
    {
      sup::dto::uint64 check = 0;
      // var size must be less than 64 bit
      ret = (memcmp(var.GetInstance(), &check, var.GetSize()) != 0);
    }
    else
    {
      log::Error("Condition::ExecuteSingleImpl - The variable %s is not scalar", varName.c_str());
    }
  }
  else
  {
    log::Error("Condition::ExecuteSingleImpl - Failed WorkSpace::GetValue(%s)", varName.c_str());
  }
  return ret ? (ExecutionStatus::SUCCESS) : (ExecutionStatus::FAILURE);
}

}  // namespace sequencer

}  // namespace sup
