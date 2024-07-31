#include "bits.h"

Bits::~Bits() {
  if (length_ > 0) delete bytes_;
  bytes_ = nullptr;
}