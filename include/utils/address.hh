#pragma once

#include <cstdint>

namespace utils {

inline uint16_t LittleEndian(uint8_t first, uint8_t second) {
  return (uint16_t)first | (uint16_t)second << 8;
}

}  // namespace utils