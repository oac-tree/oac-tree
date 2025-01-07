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

#ifndef SUP_SEQUENCER_VARIABLE_INFO_H_
#define SUP_SEQUENCER_VARIABLE_INFO_H_

#include <sup/sequencer/attribute_info.h>

#include <sup/dto/basic_scalar_types.h>

#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{
/**
 * @brief The VariableInfo class represents the static information of a Variable, including its
 * type and attributes. It also contains an index that allows to uniquely identify the Variable
 * within a workspace.
 */
class VariableInfo
{
public:
  VariableInfo(const std::string& var_type, sup::dto::uint32 idx,
               std::vector<AttributeInfo> attributes);
  ~VariableInfo();

  // Copy/move construction/assignment
  VariableInfo(const VariableInfo& other);
  VariableInfo(VariableInfo&& other);
  VariableInfo& operator=(const VariableInfo& other);
  VariableInfo& operator=(VariableInfo&& other);

  std::string GetType() const;
  sup::dto::uint32 GetIndex() const;
  std::vector<AttributeInfo> GetAttributes() const;

private:
  std::string m_var_type;
  sup::dto::uint32 m_index;
  std::vector<AttributeInfo> m_attributes;
};

/**
 * @brief Compare two VariableInfo objects for equality.
 */
bool operator==(const VariableInfo& left, const VariableInfo& right);

/**
 * @brief Compare two VariableInfo objects for inequality.
 */
bool operator!=(const VariableInfo& left, const VariableInfo& right);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_VARIABLE_INFO_H_
