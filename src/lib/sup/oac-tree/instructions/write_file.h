/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree for operational procedures
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_WRITE_FILE_H_
#define SUP_OAC_TREE_WRITE_FILE_H_

#include <sup/oac-tree/instruction.h>

namespace sup
{
namespace oac_tree
{
/**
 * @brief Instruction that writes a variable to a JSON file.
 */
class WriteFile : public Instruction
{
public:
  WriteFile();

  ~WriteFile() override;

  static const std::string Type;

private:
  void SetupImpl(const Procedure& proc) override;

  ExecutionStatus ExecuteSingleImpl(UserInterface& ui, Workspace& ws) override;

  std::string m_proc_path;
};

template <>
inline DocumentationEntry InstructionDocFor<WriteFile>()
{
  DocumentationEntry doc{{
    { Constants::INSTR_DOC_DESCRIPTION, "Instruction for writing a variable to a JSON file" }
  }};
  return doc;
}

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_WRITE_FILE_H_
