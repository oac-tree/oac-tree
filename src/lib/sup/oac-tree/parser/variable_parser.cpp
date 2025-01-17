/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/variable_registry.h>

namespace sup
{
namespace oac_tree
{
std::unique_ptr<Variable> ParseVariable(const sup::xml::TreeData& data)
{
  auto var_type = data.GetNodeName();
  if (!GlobalVariableRegistry().IsRegisteredVariableName(var_type))
  {
    std::string error_message =
      "ParseVariable(): trying to create unregistered variable with typename [" +
      var_type + "]";
    throw ParseException(error_message);
  }
  auto var = GlobalVariableRegistry().Create(var_type);
  if (!var)
  {
    std::string error_message =
      "ParseVariable(): could not create variable with typename [" +
      var_type + "]";
    throw ParseException(error_message);
  }
  var->AddAttributes(data.Attributes());
  return var;
}

}  // namespace oac_tree

}  // namespace sup
