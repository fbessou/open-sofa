#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace OpenSofa {

template <typename T>
class SyncQueue {
public:
  //! Add item at end
  void push(const T& v);

  //! Retrieve an item from the queue if any
  bool peek(T& out);

  //! Retrieve an item from the queue (wait for it)
  bool peek(T& out, unsigned long timeOutMillis);

  //! Empty the queue
  void clear();

private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

template <typename T>
void SyncQueue<T>::push(const T& v)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_queue.push(v);
  m_cond.notify_one();
}

template <typename T>
bool SyncQueue<T>::peek(T& out)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  if (m_queue.empty())
    return false;

  out = m_queue.front();
  m_queue.pop();
  return true;
}

template <typename T>
bool SyncQueue<T>::peek(T& out, unsigned long timeOutMillis)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  if (m_queue.empty())
    if (m_cond.wait_for(lock, std::chrono::milliseconds(timeOutMillis)) == std::cv_status::timeout)
      return false;

  out = m_queue.front();
  m_queue.pop();
  return true;
}

template <typename T>
void SyncQueue<T>::clear()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  m_queue.clear();
}
}
