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

#include <sup/sequencer/attribute_handler.h>

namespace sup
{
namespace sequencer
{
AttributeHandler::AttributeHandler()
  : m_attribute_definitions{}
{}

AttributeHandler::~AttributeHandler() = default;

AttributeDefinition& AttributeHandler::AddAttributeDefinition(const std::string& attr_name)
{
  // TODO: throw if an attribute with that name already exists
  m_attribute_definitions.emplace_back(attr_name);
  return m_attribute_definitions.back();
}

}  // namespace sequencer

}  // namespace sup
