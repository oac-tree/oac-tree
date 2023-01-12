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

  bool IsValid() const;

private:
  T* m_cb_manager;
  void* m_listener;
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

  bool RegisterGenericCallback(std::function<void(const std::string&, Args...)> cb, void* listener);

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
CallbackGuard<T>::CallbackGuard(T* cb_manager, void* listener)
  : m_cb_manager{cb_manager}
  , m_listener{listener}
{}

template <typename T>
CallbackGuard<T>::~CallbackGuard()
{
  if (IsValid())
  {
    m_cb_manager->UnregisterListener(m_listener);
  }
}

template <typename T>
CallbackGuard<T>::CallbackGuard(CallbackGuard&& other)
  : m_cb_manager{other.m_cb_manager}
  , m_listener(other.m_listener)
{
  other.m_listener = nullptr;
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
  std::swap(m_cb_manager, other.m_cb_manager);
  std::swap(m_listener, other.m_listener);
}

template <typename T>
bool CallbackGuard<T>::IsValid() const
{
  return m_cb_manager != nullptr && m_listener != nullptr;
}

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
CallbackGuard<NamedCallbackManager<Args...>> NamedCallbackManager<Args...>::GetCallbackGuard(
    void* listener)
{
  return CallbackGuard<NamedCallbackManager<Args...>>(this, listener);
}

}  // namespace sequencer

}  // namespace sup

#endif  // SUP_SEQUENCER_NAMED_CALLBACK_MANAGER_H_