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

/**
 * @file CallbackManager.h
 * @brief Definition of CallbackManager template class.
 * @date 24/11/21
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the CallbackManager template class.
 */

#ifndef _SEQ_CallbackManager_h_
#define _SEQ_CallbackManager_h_

#include <functional>
#include <list>
#include <mutex>

namespace sup
{
namespace sequencer
{

/**
 * @brief Threadsafe class template for managing a list of callbacks and executing them.
 */
template <typename Signature>
class CallbackManager;

template <typename... Args>
class CallbackManager<void(Args...)>
{
public:
  CallbackManager() = default;
  ~CallbackManager() = default;

  bool AddCallback(std::function<void(Args...)> cb);

  void ExecuteCallbacks(Args... args);

private:
  mutable std::mutex mtx;
  std::list<std::function<void(Args...)>> cb_list;
};

template <typename... Args>
bool CallbackManager<void(Args...)>::AddCallback(std::function<void(Args...)> cb)
{
  if (!cb)
  {
    return false;
  }
  std::lock_guard<std::mutex> lk(mtx);
  cb_list.push_back(std::move(cb));
  return true;
}

template <typename... Args>
void CallbackManager<void(Args...)>::ExecuteCallbacks(Args... args)
{
  std::lock_guard<std::mutex> lk(mtx);
  for (const auto& cb : cb_list)
  {
    cb(args...);
  }
}

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_CallbackManager_h_