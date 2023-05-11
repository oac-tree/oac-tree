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

#ifndef SUP_SEQUENCER_VALUE_MAP_INFO_H_
#define SUP_SEQUENCER_VALUE_MAP_INFO_H_

#include <sup/dto/anyvalue.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief Aggregate for the creation of a value map from string attributes. When the 'result' field
 * is false, 'failed_constraints' will contain a list of descriptions of the failed constraints.
 * Otherwise, 'value_map' will contain the required map from string to AnyValue.
 */
struct ValueMapInfo
{
  std::vector<std::string> failed_constraints;
  std::map<std::string, sup::dto::AnyValue> value_map;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_VALUE_MAP_INFO_H_
