#pragma once

#include <cstdint>

namespace utils {

inline uint8_t get_bit(uint8_t x, uint8_t n) { return (x >> n) & 1; }
inline uint8_t get_range(uint8_t x, uint8_t n, uint8_t len) {
  if (len == 1) {
    return get_bit(x, n);
  }
  uint8_t mask = 0;
  for (uint8_t i = n; i < n + len; ++i) {
    mask |= 1 << i;
  }
  return (x >> n) & mask;
}

}  // namespace utils