#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

namespace OpenSofa
{
  template <typename T>
  class SyncQueue
  {
    public:
      void push(const T& v);
      T peek();
      bool peek(T& out, unsigned long timeOutMillis);
  
    private:
	  std::queue<T> m_queue;
      std::mutex m_mutex;
	  std::condition_variable m_cond;
  };
}
