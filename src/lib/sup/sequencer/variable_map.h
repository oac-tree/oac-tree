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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_SEQUENCER_VARIABLE_MAP_H_
#define SUP_SEQUENCER_VARIABLE_MAP_H_

#include <sup/dto/basic_scalar_types.h>

#include <string>
#include <unordered_map>

namespace sup
{
namespace sequencer
{
class Workspace;

/**
 * @brief VariableMap builds a map from variable names to a unique index.
 *
 * @note This class is immutable and builds all the necessary structures during construction. This
 * allows to use VariableMap objects easily in a multithreaded context.
 */
class VariableMap
{
public:
  using VariableIndexMap = std::unordered_map<std::string, sup::dto::uint32>;

  /**
   * @brief Construct a VariableMap object from the given Workspace.
   *
   * @param workspace Workspace of variables to map.
   */
  explicit VariableMap(const Workspace& workspace);

  /**
   * @brief Destructor.
   */
  ~VariableMap();

  /**
   * @brief Return the map of variable names to their indices.
   *
   * @return map of variable names to variable indices.
   */
  const VariableIndexMap& GetVariableIndexMap() const;

  /**
   * @brief Return the number of variables that are mapped.
   *
   * @return Number of variables mapped.
   */
  sup::dto::uint32 GetNumberOfVariables() const;

  /**
   * @brief Find the variable index from the given name.
   *
   * @param var_name Variable name.
   * @return Variable index.
   *
   * @throw InvalidOperationException when variable name is not known.
   */
  sup::dto::uint32 FindVariableIndex(const std::string& var_name) const;

private:
  /**
   * @brief Initialization member function, called only during construction.
   *
   * @param workspace Workspace whose variables will be mapped.
   */
  void InitializeMap(const Workspace& workspace);
  VariableIndexMap m_variable_map;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_VARIABLE_MAP_H_
