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


#include <future>

namespace sup
{
namespace sequencer
{

class IUserInput
{
public:
  virtual ~IUserInput() = default;

  virtual int GetUserValue() const = 0;
};

// Handles only a single user input request
class AsyncUserInput
{
public:
  explicit AsyncUserInput(IUserInput& sync_input);
  ~AsyncUserInput();

  // return false if a user input request is already being managed
  bool AddUserInputRequest();

  bool UserInputRequestReady() const;

  // throws if the input was not ready or no request is active
  int GetUserInput();

private:
  IUserInput& m_sync_input;
  std::future<int> m_future;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INPUT_H_
