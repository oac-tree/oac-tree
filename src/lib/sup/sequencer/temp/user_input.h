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

#ifndef SUP_SEQUENCER_USER_INPUT_H_
#define SUP_SEQUENCER_USER_INPUT_H_

#include <sup/dto/basic_scalar_types.h>

#include <future>
#include <map>

namespace sup
{
namespace sequencer
{

class IUserInput
{
public:
  virtual ~IUserInput() = default;

  virtual int GetUserValue(sup::dto::uint64 id) = 0;

  virtual void Interrupt(sup::dto::uint64 id) = 0;
};

// Handles only a single user input request
class AsyncUserInput
{
public:
  explicit AsyncUserInput(IUserInput& sync_input);
  ~AsyncUserInput();

  // return false if a user input request is already being managed
  sup::dto::uint64 AddUserInputRequest();

  bool UserInputRequestReady(sup::dto::uint64 id) const;

  // throws if the input was not ready or no request is active
  int GetUserInput(sup::dto::uint64 id);

private:
  sup::dto::uint64 GetNewRequestId();
  IUserInput& m_sync_input;
  std::map<sup::dto::uint64, std::future<int>> m_requests;
  sup::dto::uint64 m_last_request_id;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INPUT_H_
