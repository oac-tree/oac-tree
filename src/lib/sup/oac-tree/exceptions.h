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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_EXCEPTIONS_H_
#define SUP_OAC_TREE_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace sup
{
namespace oac_tree
{
  /**
 * @brief Base Exception class with message.
 */
class MessageException : public std::exception
{
public:
  explicit MessageException(std::string message);
  MessageException(const MessageException&) = delete;
  MessageException(MessageException&&) = delete;
  MessageException& operator=(const MessageException&) = delete;
  MessageException& operator=(MessageException&&) = delete;
  const char* what() const noexcept override;
private:
  std::string m_message;
};

/**
 * @brief Generic exception thrown when an operation fails without clear reason, e.g. during a call
 * to a library method that is not under control.
 */
class RuntimeException : public MessageException
{
public:
  explicit RuntimeException(const std::string& message);
};

/**
 * @brief Exception thrown when trying to call an operation that is not supported by the object.
 */
class InvalidOperationException : public MessageException
{
public:
  explicit InvalidOperationException(const std::string& message);
};

/**
 * @brief Exception thrown when parsing to an object failed.
 */
class ParseException : public MessageException
{
public:
  explicit ParseException(const std::string& message);
};

/**
 * @brief Exception thrown when setting up a procedure failed.
 */
class ProcedureSetupException : public MessageException
{
public:
  explicit ProcedureSetupException(const std::string& message);
};

/**
 * @brief Exception thrown when setting up an instruction failed.
 */
class InstructionSetupException : public MessageException
{
public:
  explicit InstructionSetupException(const std::string& message);
};

/**
 * @brief Exception thrown when setting up a workspace variable failed.
 */
class VariableSetupException : public MessageException
{
public:
  explicit VariableSetupException(const std::string& message);
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_EXCEPTIONS_H_
