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

#ifndef SUP_SEQUENCER_APPLICATON_UTILS_H_
#define SUP_SEQUENCER_APPLICATON_UTILS_H_

#include <sup/sequencer/procedure.h>

#include <memory>
#include <string>

namespace sup
{
namespace sequencer
{
namespace utils
{

/**
 * @brief Helper class to try to load a Procedure from file and call its Setup method.
 * On failure, it will contain the reason for failure as an error message.
 *
 */
class ProcedureLoader
{
public:
  ProcedureLoader(const std::string& filename);
  ~ProcedureLoader();

  std::unique_ptr<Procedure> ParseAndSetup();

  std::string GetErrorMessage() const;
private:
  std::string m_filename;
  std::string m_error_message;
};

}  // namespace utils

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_APPLICATON_UTILS_H_
