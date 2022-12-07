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

#include "variable_parser.h"

#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/variable_registry.h>

namespace sup
{
namespace sequencer
{
std::unique_ptr<Variable> ParseVariable(const TreeData& data)
{
  auto var_type = data.GetType();
  if (!GlobalVariableRegistry().IsRegisteredVariableName(var_type))
  {
    std::string error_message =
      "sup::sequencer::ParseVariable(): trying to create unregistered variable with typename [" +
      var_type + "]";
    throw ParseException(error_message);
  }
  auto var = GlobalVariableRegistry().Create(var_type);
  if (!var)
  {
    std::string error_message =
      "sup::sequencer::ParseVariable(): could not create variable with typename [" +
      var_type + "]";
    throw ParseException(error_message);
  }
  var->AddAttributes(data.Attributes());
  return var;
}

}  // namespace sequencer

}  // namespace sup
