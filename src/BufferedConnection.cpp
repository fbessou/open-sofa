#include "BufferedConnection.hpp"
#include <cstring>

namespace OpenSofa {

BufferedConnection::QueueThread::QueueThread()
    : threadRunning_(true), thread_(&BufferedConnection::QueueThread::run, this)
{
}

BufferedConnection::QueueThread::~QueueThread()
{
  threadRunning_ = false;
  thread_.join();
}

BufferedConnection::BufferedByteInputStream::BufferedByteInputStream(ByteInputStream& inputStream)
    : inputStream_(inputStream), currentBufferCursor_(0)
{
}

void BufferedConnection::BufferedByteInputStream::run()
{
  RawBuffer buffer(RawBuffer::MaxLength);
  while (threadRunning_) {
    auto l = inputStream_.read(buffer.getData(), RawBuffer::MaxLength);
    RawBuffer buf(l);
    memcpy(buf.getData(), buffer.getData(), buf.length);
    queue_.push(buf);
  }
}

size_t BufferedConnection::BufferedByteInputStream::read(uint8_t* buf, std::size_t count)
{
  std::size_t readCount = 0;
  uint8_t* bufCursor = buf;

  // Read until max count reached or no more data in queue
  while (readCount < count && (currentBufferCursor_ < currentBuffer_.length || !queue_.empty())) {
    if (currentBufferCursor_ >= currentBuffer_.length) {
      currentBufferCursor_ = 0;

      // Read new RawBuffer
      bool success = queue_.peek(currentBuffer_);
      if (!success) {
        currentBuffer_.data.reset();
        currentBuffer_.length = 0;
        break;
      }
    }

    // Copy RawBuffer to out buf
    std::size_t toRead
        = std::min(count - readCount, (std::size_t)(currentBuffer_.length - currentBufferCursor_));
    memcpy(bufCursor, currentBuffer_.getData() + currentBufferCursor_, toRead);

    currentBufferCursor_ += toRead;
    bufCursor += toRead;
    readCount += toRead;
  }

  return readCount;
}

BufferedConnection::BufferedByteOutputStream::BufferedByteOutputStream(
    ByteOutputStream& outputStream)
    : outputStream_(outputStream)
{
}

void BufferedConnection::BufferedByteOutputStream::run()
{
  RawBuffer buffer;
  while (threadRunning_) {
    if (queue_.peek(buffer, 10)) {
      outputStream_.write(buffer.getData(), buffer.length);
    }
  }
}

size_t BufferedConnection::BufferedByteOutputStream::write(const uint8_t* buf, std::size_t count)
{
  RawBuffer buffer(count);
  memcpy(buffer.getData(), buf, count);
  queue_.push(buffer);
  return count;
}

BufferedConnection::BufferedConnection(Connection& lowerConnection)
    : inputStream_(lowerConnection.getInputStream()),
      outputStream_(lowerConnection.getOutputStream())
{
}

ByteInputStream& BufferedConnection::getInputStream()
{
  return inputStream_;
}

ByteOutputStream& BufferedConnection::getOutputStream()
{
  return outputStream_;
}
}
