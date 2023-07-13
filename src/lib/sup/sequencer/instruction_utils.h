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

#ifndef SUP_SEQUENCER_INSTRUCTION_UTILS_H_
#define SUP_SEQUENCER_INSTRUCTION_UTILS_H_

#include <sup/dto/basic_scalar_types.h>

#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
class Instruction;

namespace instruction_utils
{

const double kMaxTimeoutSeconds = 9.2e9; // More than 290 years. This should be enough...

  /**
   * @brief Get timeout attribute from an instruction and convert it from a floating point value
   * in seconds to an 64bit integer in nanoseconds.
   *
   * @param instr Instruction that holds the attribute.
   * @param attr_name Name of attribute that holds the timeout in seconds.
   *
   * @return Timeout in nanoseconds.
   *
   * @throws InstructionSetupException when the attribute is not present or its value is negative
   * or larger than the maximum (kMaxTimeoutSeconds).
   */
sup::dto::int64 GetTimeoutFromAttribute(const Instruction& instr, const std::string& attr_name);

  /**
   * @brief Get a list of variable names from an instruction attribute.
   *
   * @param instr Instruction that holds the attribute.
   * @param attr_name Name of attribute that holds a list of variable names.
   *
   * @return List of variable names.
   *
   * @throws InstructionSetupException when the attribute is not present.
   *
   * @note Variable names should be separated by VARNAME_DELIMITER, defined in constants.h.
   */
std::vector<std::string> VariableNamesFromAttribute(const Instruction& instr,
                                                    const std::string& attr_name);

}  // namespace instruction_utils

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_UTILS_H_
