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

#include <sup/oac-tree/instruction_utils.h>

#include <sup/oac-tree/constants.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/user_interface.h>

#include <cmath>

namespace sup
{
namespace oac_tree
{
namespace instruction_utils
{

bool ConvertToTimeoutNanoseconds(sup::dto::float64 timeout_sec, sup::dto::int64& timeout_ns)
{
  if (timeout_sec < 0.0 || timeout_sec > kMaxTimeoutSeconds)
  {
    return false;
  }
  timeout_ns = std::lround(timeout_sec * 1e9);
  return true;
}

bool GetVariableTimeoutAttribute(const Instruction& instr, UserInterface& ui, Workspace& ws,
                                 const std::string& attr_name, sup::dto::int64& timeout_ns)
{
  sup::dto::float64 timeout_sec = 0.0;
  if (!instr.GetAttributeValueAs(attr_name, ws, ui, timeout_sec))
  {
    return false;
  }
  if (!instruction_utils::ConvertToTimeoutNanoseconds(timeout_sec, timeout_ns))
  {
    const std::string warning_message = InstructionWarningProlog(instr) + "could not retrieve " +
      "timeout value within limits: " + std::to_string(timeout_sec);
    LogWarning(ui, warning_message);
    return false;
  }
  return true;
}

std::vector<std::string> VariableNamesFromAttribute(const Instruction& instr,
                                                    const std::string& attr_name)
{
  if (!instr.HasAttribute(attr_name))
  {
    std::string error_message =
        InstructionSetupExceptionProlog(instr)
        + "VariableNamesFromAttribute(): instruction does not have the correct attribute: ["
        + attr_name + "]";
    throw InstructionSetupException(error_message);
  }
  auto var_names_attr = instr.GetAttributeString(attr_name);
  std::vector<std::string> result;
  size_t pos = var_names_attr.find_first_not_of(DefaultSettings::VARNAME_DELIMITER);
  while (pos != std::string::npos)
  {
    auto next = var_names_attr.find_first_of(DefaultSettings::VARNAME_DELIMITER, pos);
    auto var_name = var_names_attr.substr(pos, next - pos);
    if (!var_name.empty())
    {
      result.push_back(var_name);
    }
    pos = var_names_attr.find_first_not_of(DefaultSettings::VARNAME_DELIMITER, next);
  }
  return result;
}

}  // namespace instruction_utils

}  // namespace oac_tree

}  // namespace sup
