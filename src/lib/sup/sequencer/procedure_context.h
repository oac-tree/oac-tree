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

#ifndef SUP_SEQUENCER_PROCEDURE_CONTEXT_H_
#define SUP_SEQUENCER_PROCEDURE_CONTEXT_H_

#include <string>

namespace sup
{
namespace sequencer
{
class ProcedureStore;

/**
 * @brief Structure containing the context of a procedure.
 *
 */
struct ProcedureContext
{
  std::string procedure_filename;
  const ProcedureStore* procedure_store;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_PROCEDURE_CONTEXT_H_
