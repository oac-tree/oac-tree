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
#include <list>
#include <map>

namespace sup
{
namespace sequencer
{

class IUserInput
{
public:
  virtual ~IUserInput() = default;

  // This is a blocking call to get user input. If another thread calls Interrupt() with the same
  // id, it should return within a reasonable time with an invalid reponse..
  virtual int GetUserValue(sup::dto::uint64 id) = 0;

  virtual void Interrupt(sup::dto::uint64 id) = 0;
};

// Handles only a single user input request
class AsyncUserInput
{
public:
  class Token;
  explicit AsyncUserInput(IUserInput& sync_input);
  ~AsyncUserInput();

  // return false if a user input request is already being managed
  Token AddUserInputRequest();

private:
  // query readiness of a user input request
  bool UserInputRequestReady(sup::dto::uint64 id) const;

  // throws if the input was not ready or no request is active
  int GetUserInput(sup::dto::uint64 id);

  // cancel a user input request
  void CancelInputRequest(sup::dto::uint64 id);

  sup::dto::uint64 GetNewRequestId();
  void CleanUpUnused();
  IUserInput& m_sync_input;
  std::map<sup::dto::uint64, std::future<int>> m_requests;
  sup::dto::uint64 m_last_request_id;
  std::list<sup::dto::uint64> m_cancelled;
};

class AsyncUserInput::Token
{
public:
  Token(AsyncUserInput& input_handler, sup::dto::uint64 id);
  ~Token();

  Token(const Token&) = delete;
  Token& operator=(const Token&) = delete;

  Token(Token&& other);
  Token& operator=(Token&& other) = delete;

  sup::dto::uint64 GetId() const;

  bool IsValid() const;
  bool IsReady() const;

  int GetValue();

private:
  AsyncUserInput& m_input_handler;
  sup::dto::uint64 m_id;
};

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_USER_INPUT_H_
