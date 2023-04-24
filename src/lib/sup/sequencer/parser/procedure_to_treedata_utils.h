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

#ifndef SUP_SEQUENCER_PROCEDURE_TO_TREEDATA_UTILS_H_
#define SUP_SEQUENCER_PROCEDURE_TO_TREEDATA_UTILS_H_

#include <memory>

namespace sup
{
namespace xml
{
class TreeData;
}
namespace sequencer
{
class Procedure;
class Variable;
class Instruction;
class Workspace;

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Procedure& procedure);

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Variable& variable);

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Workspace& workspace);

std::unique_ptr<sup::xml::TreeData> ToTreeData(const Instruction& instruction);

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_TO_TREEDATA_UTILS_H_
