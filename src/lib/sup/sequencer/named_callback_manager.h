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

#ifndef SUP_SEQUENCER_NAMED_CALLBACK_MANAGER_H_
#define SUP_SEQUENCER_NAMED_CALLBACK_MANAGER_H_

#include <sup/sequencer/scope_guard.h>

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
 * @note Generic callbacks will be convertible to std::function<void(const std::string&, Args...)>,
 * while callbacks for a specific name will be convertible to std::function<void(Args...)>.
 */
template <typename... Args>
class NamedCallbackManager
{
public:
  NamedCallbackManager() = default;
  ~NamedCallbackManager() = default;

  bool RegisterGenericCallback(std::function<void(const std::string&, Args...)> cb, void* listener);

  bool RegisterCallback(const std::string& name,
                        const std::function<void(Args...)>& cb, void* listener);

  bool UnregisterListener(void* listener);

  void ExecuteCallbacks(const std::string& name, Args... args);

  ScopeGuard GetCallbackGuard(void* listener);

private:
  struct GenericCallbackEntry
  {
    void* listener;
    std::function<void(const std::string&, Args...)> cb;
  };
  struct CallbackEntry
  {
    void* listener;
    std::string name;
    std::function<void(Args...)> cb;
  };
  mutable std::mutex mtx;
  std::vector<GenericCallbackEntry> generic_cb_entries;
  std::vector<CallbackEntry> cb_entries;
};

template <typename... Args>
bool NamedCallbackManager<Args...>::RegisterGenericCallback(
    std::function<void(const std::string&, Args...)> cb, void* listener)
{
  if (!cb)
  {
    return false;
  }
  std::lock_guard<std::mutex> lk(mtx);
  generic_cb_entries.push_back({listener, cb});
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
  cb_entries.push_back({listener, name, cb});
  return true;
}

template <typename... Args>
bool NamedCallbackManager<Args...>::UnregisterListener(void* listener)
{
  std::lock_guard<std::mutex> lk(mtx);
  auto result = false;
  auto new_generic_end_it = std::remove_if(generic_cb_entries.begin(), generic_cb_entries.end(),
                                           [listener](GenericCallbackEntry cb_entry)
                                           { return cb_entry.listener == listener; });
  result = new_generic_end_it != generic_cb_entries.end();
  generic_cb_entries.erase(new_generic_end_it, generic_cb_entries.end());
  auto new_end_it =
      std::remove_if(cb_entries.begin(), cb_entries.end(),
                     [listener](CallbackEntry cb_entry) { return cb_entry.listener == listener; });
  result = result || new_end_it != cb_entries.end();
  cb_entries.erase(new_end_it, cb_entries.end());
  return result;
}

template <typename... Args>
void NamedCallbackManager<Args...>::ExecuteCallbacks(const std::string& name, Args... args)
{
  std::lock_guard<std::mutex> lk(mtx);
  for (const auto& cb_entry : generic_cb_entries)
  {
    cb_entry.cb(name, args...);
  }
  for (const auto& cb_entry : cb_entries)
  {
    if (cb_entry.name == name)
    {
      cb_entry.cb(args...);
    }
  }
}

template <typename... Args>
ScopeGuard NamedCallbackManager<Args...>::GetCallbackGuard(
    void* listener)
{
  auto unregister = [this, listener](){
    if (listener != nullptr)
    {
      UnregisterListener(listener);
    }
  };
  return ScopeGuard(unregister);
}

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_NAMED_CALLBACK_MANAGER_H_