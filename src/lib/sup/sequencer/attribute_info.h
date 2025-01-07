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

#ifndef SUP_SEQUENCER_ATTRIBUTE_INFO_H_
#define SUP_SEQUENCER_ATTRIBUTE_INFO_H_

#include <sup/sequencer/constraint.h>

#include <string>
#include <vector>
#include <utility>

namespace sup
{
namespace sequencer
{
using AttributeInfo = std::pair<std::string, std::string>;

std::vector<AttributeInfo> ToAttributeInfos(const StringAttributeList& attrs);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_ATTRIBUTE_INFO_H_
