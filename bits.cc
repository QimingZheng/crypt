#include "bits.h"

Bits::Bits() {
  capacity_ = 0;
  length_ = 0;
  bytes_ = nullptr;
}

Bits Bits::FromBinaryString(const std::string& str) {
  Bits bits;
  bits.length_ = str.length();
  bits.capacity_ = (str.length() + kBitsOfOneByte - 1) / kBitsOfOneByte;
  if (bits.length_ > 0) {
    bits.bytes_ = new byte[bits.capacity_];
    std::memset(bits.bytes_, 0, sizeof(byte) * bits.capacity_);
    for (size_t i = 0; i < str.length(); i++) {
      if (str[str.length() - 1 - i] == '1')
        bits.bytes_[bits.capacity_ - 1 - i / kBitsOfOneByte] |=
            (1 << (i % kBitsOfOneByte));
    }
  }
  return bits;
}

Bits::Bits(const Bits& other) {
  if (other.length_ > 0) {
    capacity_ = other.capacity_;
    bytes_ = new byte[capacity_];
    length_ = other.length_;
    std::memcpy(bytes_, other.bytes_, sizeof(byte) * capacity_);
  }
}

Bits::~Bits() {
  if (capacity_ > 0) delete bytes_;
  bytes_ = nullptr;
  capacity_ = 0;
  length_ = 0;
}

bool Bits::operator==(const Bits& bits) const {
  if (length_ != bits.length_) return false;
  if (length_ == 0) return true;
  for (size_t i = capacity_ - 1; i > capacity_ - UsedBytes(); i--) {
    if ((bytes_[i] ^ bits.bytes_[i]) != 0) return false;
  }
  if ((bytes_[capacity_ - UsedBytes()] ^ bits.bytes_[capacity_ - UsedBytes()]) &
      (0xf >> (kBitsOfOneByte - length_ % kBitsOfOneByte))) {
    return false;
  }
  return true;
}

std::string Bits::ToString() const {
  std::string str = "";
  for (size_t i = 0; i < length_; i++) {
    str +=
        (bytes_[capacity_ - 1 - i / kBitsOfOneByte] >> (i % kBitsOfOneByte)) & 1
            ? '1'
            : '0';
  }
  std::reverse(str.begin(), str.end());
  return str;
}

void Bits::operator<<=(size_t offset) {
  assert(offset > 0);
  if (length_ == 0) return;
  size_t needed_bytes =
      (length_ + offset + kBitsOfOneByte - 1) / kBitsOfOneByte;
  if (needed_bytes > capacity_) {
    byte* bytes = new byte[needed_bytes];
    std::memset(bytes, 0, sizeof(byte) * needed_bytes);
    if (capacity_ > 0) {
      std::memcpy(&(bytes[needed_bytes - capacity_]), bytes_,
                  capacity_ * sizeof(byte));
      delete bytes_;
    }
    bytes_ = bytes;
    capacity_ = needed_bytes;
  }
  for (size_t i = 0; i < capacity_; i++) {
    bytes_[i] <<= offset;
    if (offset >= kBitsOfOneByte) {
      if (i + (offset - kBitsOfOneByte + kBitsOfOneByte - 1) / kBitsOfOneByte <
          capacity_) {
        bytes_[i] |=
            (bytes_[i + (offset - kBitsOfOneByte + kBitsOfOneByte - 1) /
                            kBitsOfOneByte]
             << ((offset - kBitsOfOneByte) % kBitsOfOneByte));
      }
    }
    if (i + (offset + kBitsOfOneByte - 1) / kBitsOfOneByte < capacity_) {
      bytes_[i] |=
          (bytes_[i + (offset + kBitsOfOneByte - 1) / kBitsOfOneByte] >>
           (kBitsOfOneByte - (offset % kBitsOfOneByte)));
    }
  }
  length_ += offset;
}

void Bits::operator>>=(size_t offset) {
  assert(offset > 0);
  if (length_ <= offset) {
    length_ = 0;
    return;
  }
  for (size_t i = capacity_ - 1; i != size_t(-1); i--) {
    bytes_[i] >>= offset;
    if (offset >= kBitsOfOneByte) {
      if (i >=
          (offset - kBitsOfOneByte + kBitsOfOneByte - 1) / kBitsOfOneByte) {
        bytes_[i] |=
            (bytes_[i - (offset - kBitsOfOneByte + kBitsOfOneByte - 1) /
                            kBitsOfOneByte] >>
             (((offset - kBitsOfOneByte) % kBitsOfOneByte)));
      }
    }
    if (i >= (offset + kBitsOfOneByte - 1) / kBitsOfOneByte)
      bytes_[i] |= (bytes_[i - (offset + kBitsOfOneByte - 1) / kBitsOfOneByte]
                    << (kBitsOfOneByte - (offset % kBitsOfOneByte)));
  }
  if (offset > length_)
    length_ = 0;
  else
    length_ -= offset;
}

bool Bits::operator[](size_t index) {
  if (index >= length_) {
    throw std::runtime_error("index out of bound");
  }
  return (bytes_[capacity_ - 1 - index / kBitsOfOneByte] >>
          (index % kBitsOfOneByte)) &
         1;
}

void Bits::operator^=(const Bits& other) {
  if (other.length_ == 0) return;
  if (length_ == 0) {
    length_ = other.length_;
    capacity_ = other.capacity_;
    bytes_ = new byte[capacity_];
    std::memset(bytes_, 0, capacity_);
    std::memcpy(bytes_, other.bytes_, capacity_ * sizeof(byte));
    return;
  }
  if (length_ < other.length_ && capacity_ < other.UsedBytes()) {
    byte* bytes = new byte[other.capacity_];
    std::memset(bytes, 0, other.capacity_);
    if (length_ > 0) {
      std::memcpy(&bytes[other.capacity_ - capacity_], bytes_,
                  capacity_ * sizeof(byte));
      delete bytes_;
    }
    bytes_ = bytes;
    capacity_ = other.capacity_;
  }
  size_t i = 0, j = 0;
  while (i < UsedBytes() && j < other.UsedBytes()) {
    bytes_[capacity_ - 1 - i] ^= other.bytes_[other.capacity_ - 1 - j];
    i++;
    j++;
  }
  while (j < other.UsedBytes()) {
    bytes_[capacity_ - 1 - j] ^= other.bytes_[other.capacity_ - 1 - j];
    j++;
  }
  length_ = std::max(length_, other.length_);
}

Bits Bits::operator||(const Bits& other) {
  Bits bits = *this;
  bits <<= (other.length_);
  bits ^= other;
  return bits;
}

size_t Bits::UsedBytes() const {
  return (length_ + (kBitsOfOneByte - 1)) / kBitsOfOneByte;
}
