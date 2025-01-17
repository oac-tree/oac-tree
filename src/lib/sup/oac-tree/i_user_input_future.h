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

#ifndef SUP_OAC_TREE_I_USER_INPUT_FUTURE_H_
#define SUP_OAC_TREE_I_USER_INPUT_FUTURE_H_

#include <sup/oac-tree/user_input_reply.h>

#include <sup/dto/anyvalue.h>

namespace sup
{
namespace oac_tree
{

/**
 * @brief IUserInputFuture is returned when requesting user input asynchronously. It allows the
 * client to regularly poll to see if the user input is ready and then retrieve it.
 *
 * @note Implementations may throw exceptions when trying to get the value when it is not ready.
 *
 */
class IUserInputFuture
{
public:
  virtual ~IUserInputFuture();

  /**
   * @brief Get the unique identifier for this input request.
   *
   * @return Unique identifier.
   */
  virtual sup::dto::uint64 GetId() const = 0;

  /**
   * @brief Query if this future is valid, i.e. it refers to a real input request.
   *
   * @return true when the future is valid.
   */
  virtual bool IsValid() const = 0;

  /**
   * @brief Query if the value of this future is ready, i.e. it can be retrieved using GetValue.
   *
   * @return true when the future is ready.
   */
  virtual bool IsReady() const = 0;

  /**
   * @brief Wait with a timeout for the future to become ready.
   *
   * @param seconds Timeout in seconds.
   *
   * @return True when ready within the timeout, false otherwise.
   */
   virtual bool WaitFor(double seconds) const = 0;

  /**
   * @brief Get the value of the future if it is ready.
   *
   * @return Value from the user when it is ready.
   */
  virtual UserInputReply GetValue() = 0;
};

/**
 * @brief This implementation of IUserInputFuture is used when user input is not supported. It will
 * always be invalid and not ready. GetValue() will always throw.
 *
 */
class UnsupportedInputFuture : public IUserInputFuture
{
public:
  UnsupportedInputFuture();
  ~UnsupportedInputFuture();

  sup::dto::uint64 GetId() const override;

  bool IsValid() const override;
  bool IsReady() const override;
  bool WaitFor(double seconds) const override;

  UserInputReply GetValue() override;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_I_USER_INPUT_FUTURE_H_
