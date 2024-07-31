#pragma once

#include <stdlib.h>

typedef unsigned char byte;

class Bits {
 public:
  Bits();
  ~Bits();

 private:
  // length in bit.
  size_t length_ = 0;
  // Stores bits in little-endian format.
  byte* bytes_ = nullptr;
};
