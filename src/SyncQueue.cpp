#include "SyncQueue.hpp"

namespace OpenSofa
{
  template <typename T>
  void SyncQueue::push(const T& v)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(v);
	m_cond.notify_one();
  }

  template <typename T>
  T SyncQueue::peek()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    while(m_queue.empty())
      m_cond.wait(m_mutex);
    return m_queue.pop();
  }

  template <typename T>
  bool SyncQueue::peek(T& out, unsigned long timeOutMillis)
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    
	if(m_queue.empty())
      if(m_cond.wait(m_mutex, std::chorno::millisecond(timeOutMillis)) == std::cv_status::timeout)
        return false;
    
    T = m_queue.pop();
    return true;
  }
}
