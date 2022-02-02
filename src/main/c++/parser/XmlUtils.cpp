/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "XmlUtils.h"

namespace sup
{
namespace sequencer
{
//! Converts xmlChar to std::string.
std::string ToString(const xmlChar *xml_name)
{
  return std::string(reinterpret_cast<const char *>(xml_name), xmlStrlen(xml_name));
}

//! Converts std::string to xmlChar.
const xmlChar *FromString(const std::string &str)
{
  return reinterpret_cast<const xmlChar *>(str.c_str());
}

}  // namespace sequencer

}  // namespace sup
