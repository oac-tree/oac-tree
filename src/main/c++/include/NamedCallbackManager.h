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
 * @brief Guard class that unregisters callbacks during destruction.
 */
template <typename T>
class CallbackGuard
{
public:
  CallbackGuard(T* cb_manager, void *listener);
  ~CallbackGuard();

  CallbackGuard(CallbackGuard&& other);
  CallbackGuard& operator=(CallbackGuard&& other);

  CallbackGuard(const CallbackGuard& other) = delete;
  CallbackGuard& operator=(const CallbackGuard& other) = delete;

  void Swap(CallbackGuard& other);

private:
  T* cb_manager;
  void* listener;
};

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

  bool AddGenericCallback(std::function<void(const std::string&, Args...)> cb, void* listener);

  bool RegisterCallback(const std::string& name,
                        const std::function<void(Args...)>& cb, void* listener);

  bool UnregisterListener(void* listener);

  void ExecuteCallbacks(const std::string& name, Args... args);

  CallbackGuard<NamedCallbackManager<Args...>> GetCallbackGuard(void* listener);

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

template <typename T>
CallbackGuard<T>::CallbackGuard(T* cb_manager_, void* listener_)
  : cb_manager{cb_manager_}
  , listener{listener_}
{}

template <typename T>
CallbackGuard<T>::~CallbackGuard()
{
  if (listener)
  {
    cb_manager->UnregisterListener(listener);
  }
}

template <typename T>
CallbackGuard<T>::CallbackGuard(CallbackGuard&& other)
  : cb_manager{other.cb_manager}
  , listener(other.listener)
{
  other.listener = nullptr;
}

template <typename T>
CallbackGuard<T>& CallbackGuard<T>::operator=(CallbackGuard&& other)
{
  CallbackGuard<T> moved{std::move(other)};
  Swap(moved);
  return *this;
}

template <typename T>
void CallbackGuard<T>::Swap(CallbackGuard& other)
{
  std::swap(cb_manager, other.cb_manager);
  std::swap(listener, other.listener);
}

template <typename... Args>
bool NamedCallbackManager<Args...>::AddGenericCallback(
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
CallbackGuard<NamedCallbackManager<Args...>> NamedCallbackManager<Args...>::GetCallbackGuard(
    void* listener)
{
  return CallbackGuard<NamedCallbackManager<Args...>>(this, listener);
}

}  // namespace sequencer

}  // namespace sup

#endif  // _SEQ_NamedCallbackManager_h_