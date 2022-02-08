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

#ifndef _SEQ_ProcedureToTreeDataUtils_h_
#define _SEQ_ProcedureToTreeDataUtils_h_

#include <memory>

namespace sup
{
namespace sequencer
{

class TreeData;
class Procedure;
class Variable;
class Instruction;

std::unique_ptr<TreeData> ToTreeData(const Procedure& procedure);

std::unique_ptr<TreeData> ToTreeData(const Variable& variable);

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_ProcedureToTreeDataUtils_h_
