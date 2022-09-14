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

#ifndef SUP_SEQUENCER_SEQUENCE_PARSER_IMPL_H_
#define SUP_SEQUENCER_SEQUENCE_PARSER_IMPL_H_

#include <sup/sequencer/tree_data.h>

#include <memory>

namespace sup
{
namespace sequencer
{
std::unique_ptr<TreeData> ParseXMLDataFile(const std::string& filename);

std::unique_ptr<TreeData> ParseXMLDataString(const std::string& xml_str);
}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_SEQUENCE_PARSER_IMPL_H_
