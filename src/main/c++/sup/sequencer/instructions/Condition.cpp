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

// Global header files

#include <common/AnyType.h>
#include <common/AnyValue.h>
#include <common/BasicTypes.h>

#include <sup/sequencer/log.h>

// Local header files

#include "Condition.h"
#include <sup/sequencer/workspace.h>

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

const std::string Condition::Type = "Condition";

// Function declaration

// Function definition

Condition::Condition() : Instruction(Type) {}

Condition::~Condition() = default;

ExecutionStatus Condition::ExecuteSingleImpl(UserInterface * /*ui*/, Workspace *ws)
{
  ::ccs::types::AnyValue var;
  std::string varName = GetAttribute("var_name");
  bool ret = ws->GetValue(varName, var);

  if (ret)
  {
    ::ccs::base::SharedReference<const ::ccs::types::ScalarType> varType = var.GetType();
    ret = varType.IsValid();
    if (ret)
    {
      ::ccs::types::uint64 check = 0;
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

extern "C"
{
  // C API function definitions

}  // extern C
