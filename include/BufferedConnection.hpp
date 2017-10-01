#pragma once

#include "Connection.hpp"
#include "RawBuffer.hpp"
#include "SyncQueue.hpp"
#include <cstdint>
#include <memory>
#include <thread>

namespace OpenSofa {
class BufferedConnection : public Connection {
private:
  class QueueThread {
  public:
    QueueThread();
    ~QueueThread();

  protected:
    virtual void run() = 0;

    bool threadRunning_;
    SyncQueue<RawBuffer> queue_;
    std::thread thread_;
  };

  class BufferedByteInputStream : public ByteInputStream, private QueueThread {
  public:
    BufferedByteInputStream(ByteInputStream& inputStream);
    size_t read(uint8_t* buf, std::size_t count);
    void close()
    {
    }

  private:
    void run();
    ByteInputStream& inputStream_;
    uint32_t currentBufferCursor_;
    RawBuffer currentBuffer_;
  };

  class BufferedByteOutputStream : public ByteOutputStream, private QueueThread {
  public:
    BufferedByteOutputStream(ByteOutputStream& outputStream);
    size_t write(const uint8_t* buf, std::size_t count);
    void close()
    {
    }

  private:
    void run();
    ByteOutputStream& outputStream_;
  };

public:
  BufferedConnection(Connection& lowerConnection);
  ~BufferedConnection();

  ByteInputStream& getInputStream();
  ByteOutputStream& getOutputStream();

private:
  BufferedByteInputStream inputStream_;
  BufferedByteOutputStream outputStream_;
};
}
