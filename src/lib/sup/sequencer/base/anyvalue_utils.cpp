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

#include <sup/sequencer/anyvalue_utils.h>

namespace sup
{
namespace sequencer
{
namespace utils
{

bool ValidateMemberType(const sup::dto::AnyValue& anyvalue, const std::string& mem_name,
                        const sup::dto::AnyType& mem_type)
{
  if (!anyvalue.HasField(mem_name))
  {
    return false;
  }
  if (anyvalue[mem_name].GetType() != mem_type)
  {
    return false;
  }
  return true;
}

}  // namespace utils

}  // namespace sequencer

}  // namespace sup
