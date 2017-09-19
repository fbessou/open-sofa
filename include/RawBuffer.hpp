#pragma once

#include <memory>
#include <cstdint>

struct RawBuffer {
  static const uint32_t MaxLength = 1024;

  RawBuffer() : length(0), data(nullptr) {}

  const void* getData() const { return data.get(); }
  // void* getData() { return data.get(); }

  std::uint32_t length;
  std::shared_ptr<void> data;
};

