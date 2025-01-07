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

#ifndef SUP_SEQUENCER_SETUP_TEARDOWN_ACTIONS_H_
#define SUP_SEQUENCER_SETUP_TEARDOWN_ACTIONS_H_

#include <functional>
#include <string>

namespace sup
{
namespace sequencer
{

/**
 * @brief Structure that holds optional setup/teardown actions with an identifier.
 *
 * @note Objects with an empty identifier will typically be ignored. This includes the default
 * constructed objects.
 */
struct SetupTeardownActions
{
  std::string m_identifier;
  std::function<void()> m_setup;
  std::function<void()> m_teardown;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_SETUP_TEARDOWN_ACTIONS_H_
