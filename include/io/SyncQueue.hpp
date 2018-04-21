#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace OpenSofa {
namespace io {

template <typename T>
class SyncQueue {
public:
  //! Add item at end
  void push(const T& v);
  void push(T&& v);

  //! Retrieve an item from the queue if any
  bool peek(T& out);

  //! Retrieve an item from the queue (wait for it)
  bool peek(T& out, unsigned long timeOutMillis);

  //! Empty the queue
  void clear();

  //! Indicate whether it is empty
  bool empty();

private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

template <typename T>
void SyncQueue<T>::push(const T& v)
{
  std::unique_lock<std::mutex> lock(mutex_);
  queue_.push(v);
  cond_.notify_one();
}

template <typename T>
void SyncQueue<T>::push(T&& v)
{
  std::unique_lock<std::mutex> lock(mutex_);
  queue_.push(std::move(v));
  cond_.notify_one();
}

template <typename T>
bool SyncQueue<T>::peek(T& out)
{
  std::unique_lock<std::mutex> lock(mutex_);

  if (queue_.empty())
    return false;

  out = std::move(queue_.front());
  queue_.pop();
  return true;
}

template <typename T>
bool SyncQueue<T>::peek(T& out, unsigned long timeOutMillis)
{
  std::unique_lock<std::mutex> lock(mutex_);

  if (queue_.empty())
    if (cond_.wait_for(lock, std::chrono::milliseconds(timeOutMillis)) == std::cv_status::timeout)
      return false;

  out = queue_.front();
  queue_.pop();
  return true;
}

template <typename T>
void SyncQueue<T>::clear()
{
  std::unique_lock<std::mutex> lock(mutex_);

  queue_.clear();
}

template <typename T>
bool SyncQueue<T>::empty()
{
  std::unique_lock<std::mutex> lock(mutex_);
  return queue_.empty();
}
}}
