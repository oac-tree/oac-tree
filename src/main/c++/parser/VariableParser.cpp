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

#include <common/AnyTypeHelper.h>
#include <common/AnyValueHelper.h>

#include "log.h"

// Local header files

#include "VariableParser.h"
#include "VariableRegistry.h"

// Type definition

namespace sup
{
namespace sequencer
{
// Global variables

// Function declaration

// Function definition

std::unique_ptr<Variable> ParseVariable(const TreeData& data)
{
  auto var_type = data.GetType();
  auto var = GlobalVariableRegistry().Create(var_type);

  if (!var)
  {
    log::Warning("sup::sequencer::ParseVariable() - couldn't parse variable with type: '%s'",
                var_type.c_str());
    return {};
  }

  log::Debug("sup::sequencer::ParseVariable() - parsing attributes for variable of type: '%s'",
            var_type.c_str());
  var->AddAttributes(data.Attributes());

  return var;
}

}  // namespace sequencer

}  // namespace sup

extern "C"
{
  // C API function definitions

}  // extern C
