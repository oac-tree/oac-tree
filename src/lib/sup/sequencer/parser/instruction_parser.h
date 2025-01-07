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

#ifndef SUP_SEQUENCER_INSTRUCTION_PARSER_H_
#define SUP_SEQUENCER_INSTRUCTION_PARSER_H_

#include <sup/sequencer/instruction.h>

#include <sup/xml/tree_data.h>

#include <memory>

namespace sup
{
namespace sequencer
{
/**
 * @brief Parse to Instruction.
 */
std::unique_ptr<Instruction> ParseInstruction(const sup::xml::TreeData& data);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_INSTRUCTION_PARSER_H_
