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
 * @file NamedCallbackManager.h
 * @brief Definition of NamedCallbackManager template class.
 * @date 03/02/22
 * @author Walter Van Herck (IO)
 * @copyright 2010-2022 ITER Organization
 * @details This header file contains the definition of the NamedCallbackManager template class.
 */

#ifndef _SEQ_NamedCallbackManager_h_
#define _SEQ_NamedCallbackManager_h_

#include <algorithm>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

namespace sup
{
namespace sequencer
{

/**
 * @brief Threadsafe class template for managing a list of callbacks and executing them.
 *
 * @note Generic callbacks will be std::function<void(const std::string&, Args...)>
 */
template <typename... Args>
class NamedCallbackManager
{
public:
  NamedCallbackManager() = default;
  ~NamedCallbackManager() = default;

  bool AddGenericCallback(std::function<void(const std::string&, Args...)> cb);

  bool RegisterCallback(const std::string& name,
                        const std::function<void(Args...)>& cb, void* listener);

  bool UnregisterListener(void* listener);

  void ExecuteCallbacks(const std::string& name, Args... args);

private:
  struct CallbackEntry
  {
    void* listener;
    std::string name;
    std::function<void(Args...)> cb;
  };
  mutable std::mutex mtx;
  std::vector<std::function<void(const std::string&, Args...)>> generic_cb_list;
  std::vector<CallbackEntry> cb_entries;
};

template <typename... Args>
bool NamedCallbackManager<Args...>::AddGenericCallback(
    std::function<void(const std::string&, Args...)> cb)
{
  if (!cb)
  {
    return false;
  }
  std::lock_guard<std::mutex> lk(mtx);
  generic_cb_list.push_back(std::move(cb));
  return true;
}

template <typename... Args>
bool NamedCallbackManager<Args...>::RegisterCallback(
    const std::string& name, const std::function<void(Args...)>& cb,
    void* listener)
{
  if (!cb)
  {
    return false;
  }
  std::lock_guard<std::mutex> lk(mtx);
  cb_entries.emplace_back({listener, name, cb});
  return true;
}

template <typename... Args>
bool NamedCallbackManager<Args...>::UnregisterListener(void* listener)
{
  std::lock_guard<std::mutex> lk(mtx);
  auto new_end_it = std::remove_if(cb_entries.begin(), cb_entries.end(),
                                   [listener](decltype(cb_entries.begin()) it)
                                   { return it->listener = listener; });
  auto result = new_end_it == cb_entries.end();
  cb_entries.erase(new_end_it, cb_entries.end());
  return result;
}

template <typename... Args>
void NamedCallbackManager<Args...>::ExecuteCallbacks(const std::string& name, Args... args)
{
  std::lock_guard<std::mutex> lk(mtx);
  for (const auto& cb : generic_cb_list)
  {
    cb(name, args...);
  }
  for (const auto& cb_entry : cb_entries)
  {
    if (cb_entry.name == name)
    {
      cb_entry.cb(args...);
    }
  }
}

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_NamedCallbackManager_h_