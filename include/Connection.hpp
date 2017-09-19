#pragma once

#include "SyncQueue.hpp"
#include <functional>
#include <memory>
#include <thread>

namespace OpenSofa {
template <typename T>
class Connection {
public:
  typedef std::function<void(const T&)> SendFunc;
  typedef std::function<bool(T&)> RecvFunc;

  Connection(const SendFunc& sendFunc, const RecvFunc& recvFunc);
  ~Connection();

  void send(const T& buffer);
  void recv(T& buffer);

private:
  void runSend();
  void runRecv();

  bool sendThreadRunning_;
  bool recvThreadRunning_;
  std::thread sendThread_;
  std::thread recvThread_;
  SyncQueue<T> sendQueue_;
  SyncQueue<T> recvQueue_;
  SendFunc send_;
  RecvFunc recv_;
};

template <typename T>
Connection<T>::Connection(const SendFunc& sendFunc, const RecvFunc& recvFunc)
    : sendThreadRunning_(true)
    , recvThreadRunning_(true)
    , sendThread_(&Connection<T>::runSend, this)
    , recvThread_(&Connection<T>::runRecv, this)
    , send_(sendFunc)
    , recv_(recvFunc)
{
}

template <typename T>
Connection<T>::~Connection()
{
  sendThreadRunning_ = false;
  recvThreadRunning_ = false;
  sendThread_.join();
  recvThread_.join();
}

template <typename T>
void Connection<T>::send(const T& buffer)
{
  sendQueue_.push(buffer);
}

template <typename T>
void Connection<T>::recv(T& buffer)
{
  buffer = recvQueue_.peek();
}

template <typename T>
void Connection<T>::runSend()
{
  while (sendThreadRunning_) {
    T buffer = sendQueue_.peek();
    send_(buffer);
  }
}

template <typename T>
void Connection<T>::runRecv()
{
  T buffer;
  while (recvThreadRunning_) {
    if (recv_(buffer))
      recvQueue_.push(buffer);
  }
}
}
