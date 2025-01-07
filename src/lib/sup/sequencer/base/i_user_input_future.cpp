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

#include <sup/sequencer/i_user_input_future.h>

#include <sup/sequencer/exceptions.h>

namespace sup
{
namespace sequencer
{

IUserInputFuture::~IUserInputFuture() = default;

UnsupportedInputFuture::UnsupportedInputFuture() = default;

UnsupportedInputFuture::~UnsupportedInputFuture() = default;

sup::dto::uint64 UnsupportedInputFuture::GetId() const
{
  return 0;
}

bool UnsupportedInputFuture::IsValid() const
{
  return false;
}

bool UnsupportedInputFuture::IsReady() const
{
  return false;
}

bool UnsupportedInputFuture::WaitFor(double seconds) const
{
  (void)seconds;
  return false;
}

sup::sequencer::UserInputReply UnsupportedInputFuture::GetValue()
{
  const std::string error =
    "UnsupportedInputFuture::GetValue(): this user interface does not support user input";
  throw InvalidOperationException(error);
}

}  // namespace sequencer

}  // namespace sup
