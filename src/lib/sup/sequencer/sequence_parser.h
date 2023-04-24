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

#ifndef SUP_SEQUENCER_SEQUENCE_PARSER_H_
#define SUP_SEQUENCER_SEQUENCE_PARSER_H_

#include <sup/sequencer/procedure.h>

#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
void LoadPlugin(const std::string& name);

std::unique_ptr<Procedure> ParseProcedureFile(const std::string& filename);

std::unique_ptr<Procedure> ParseProcedureString(const std::string& xml_str);

std::string GetXMLString(const Procedure& procedure);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_SEQUENCE_PARSER_H_
