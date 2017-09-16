#pragma once

#include <memory>
#include <thread>
#include "SyncQueue.hpp"

namespace OpenSofa
{
  template <typename T>
  class IOThread
  {
    public:
      SyncQueue<std::shared_ptr<T>> m_in_queue;
      SyncQueue<std::shared_ptr<T>> m_out_queue;

      IOThread();
      ~IOThread();

      void runIn();
      void runOut();
    private:
      bool m_in_running, m_out_runnig;
      std::thread m_in, m_out;
  };


  template <typename T>
  IOThread<T>::IOThread() :
    m_in_running(true),
    m_out_running(true),
    m_in(run, this),
    m_out(run, this)
  {
  }

  template <typename T>
  IOThread<T>::~IOThread()
  {
    m_in_running = false;
    m_out_running = false;
    m_in.join();
    m_out.join();
  }

  template <typename T>
  void IOThread<T>::runIn()
  {
    while(m_in_running) {
      std::shared_ptr<T> data_ptr;
      if(read(data_ptr)) // READ ptr func?
        m_in_queue.push(data_ptr);
    }
  }

  template <typename T>
  void IOThread<T>::runOut()
  {
    while(m_out_running) {
      std::shared_ptr<T> data_ptr = m_out_queue.peek();
      send(data_ptr); // SEND ptr func?
    }
  }
}
