#pragma once

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <algorithm>
#include <memory>
#include <string>

typedef unsigned char byte;
constexpr int kBitsOfOneByte = sizeof(byte) * 8;

class Bits {
 public:
  Bits();
  ~Bits();
  Bits(const Bits& other);

  static Bits FromBinaryString(const std::string& str);
  std::string ToString() const;

  bool operator==(const Bits& bits) const;

  void operator<<=(size_t offset);
  void operator>>=(size_t offset);
  void operator^=(const Bits& other);
  Bits operator||(const Bits& other);

  bool operator[](size_t index);

  bool GetLeftMost() const;
  bool GetRightMost() const;

 private:
  size_t UsedBytes() const;
  // length in bit.
  size_t length_ = 0;
  // capacity in bytes.
  size_t capacity_ = 0;
  // Stores bits in little-endian format.
  byte* bytes_ = nullptr;
};
