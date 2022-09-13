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

/**
 * @file XmlUtils.h
 * @brief Xml-related utility functions.
 */

#ifndef _SEQ_XmlUtils_h_
#define _SEQ_XmlUtils_h_

#include <libxml/xmlstring.h>

#include <string>

namespace sup
{
namespace sequencer
{

//! Converts xmlChar to std::string.
std::string ToString(const xmlChar *xml_name);

//! Converts std::string to xmlChar.
const xmlChar *FromString(const std::string &str);

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_TreeDataXmlWriteUtils_h_
