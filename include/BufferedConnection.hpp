#pragma once

#include "SyncQueue.hpp"
#include <functional>
#include <memory>
#include <thread>

namespace OpenSofa {
template <typename T>
class BufferedConnection {
public:
  typedef std::function<void(const T&)> SendFunc;
  typedef std::function<bool(T&)> RecvFunc;

  BufferedConnection(const SendFunc& sendFunc, const RecvFunc& recvFunc);
  ~BufferedConnection();

  void send(const T& buffer);

  //! Read a pending buffer if any
  bool recv(T& buffer);
  //! Read a buffer if any, or wait for it
  bool recv(T& buffer, unsigned long timeOutMillis);

private:
  void runSend();
  void runRecv();

  bool sendThreadRunning_;
  bool recvThreadRunning_;
  SendFunc send_;
  RecvFunc recv_;
  SyncQueue<T> sendQueue_;
  SyncQueue<T> recvQueue_;
  std::thread sendThread_;
  std::thread recvThread_;
};

template <typename T>
BufferedConnection<T>::BufferedConnection(const SendFunc& sendFunc, const RecvFunc& recvFunc)
    : sendThreadRunning_(true)
    , recvThreadRunning_(true)
    , send_(sendFunc)
    , recv_(recvFunc)
    , sendThread_(&BufferedConnection<T>::runSend, this)
    , recvThread_(&BufferedConnection<T>::runRecv, this)
{
}

template <typename T>
BufferedConnection<T>::~BufferedConnection()
{
  sendThreadRunning_ = false;
  recvThreadRunning_ = false;
  sendThread_.join();
  recvThread_.join();
}

template <typename T>
void BufferedConnection<T>::send(const T& buffer)
{
  sendQueue_.push(buffer);
}

template <typename T>
bool BufferedConnection<T>::recv(T& buffer)
{
  return recvQueue_.peek(buffer);
}

template <typename T>
bool BufferedConnection<T>::recv(T& buffer, unsigned long timeOutMillis)
{
  return recvQueue_.peek(buffer, timeOutMillis);
}

template <typename T>
void BufferedConnection<T>::runSend()
{
  T buffer;
  while (sendThreadRunning_) {
    if (sendQueue_.peek(buffer, 10))
      send_(buffer);
  }
}

template <typename T>
void BufferedConnection<T>::runRecv()
{
  T buffer;
  while (recvThreadRunning_) {
    if (recv_(buffer))
      recvQueue_.push(buffer);
  }
}
}
