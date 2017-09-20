#pragma once

#include <cstdint>
#include <memory>

struct RawBuffer {
  static const uint32_t MaxLength = 1024;

  RawBuffer()
      : length(0)
      , data(nullptr)
  {
  }
  RawBuffer(std::uint32_t l)
      : length(l)
      , data(new char[l])
  {
  }

  const void* getData() const { return data.get(); }
  void* getData() { return data.get(); }

  std::uint32_t length;
  std::shared_ptr<void> data;
};
