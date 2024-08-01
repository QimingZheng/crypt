#include "bits.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Bits, Bits) {
  Bits empty;
  Bits bits;
  bits <<= 10;
  EXPECT_EQ(bits, empty);
}

TEST(Bits, FromBinaryString) {
  Bits bits = Bits::FromBinaryString("101010010001");
  EXPECT_EQ(bits[0], true);
  EXPECT_EQ(bits[1], false);
  EXPECT_EQ(bits[2], false);
  EXPECT_EQ(bits[4], true);
  EXPECT_EQ(bits[7], true);
  EXPECT_EQ(bits[9], true);
  EXPECT_EQ(bits[11], true);
}

TEST(Bits, LeftShift) {
  Bits bits = Bits::FromBinaryString("101010010001");
  bits <<= 10;
  EXPECT_EQ(bits[10], true);
  EXPECT_EQ(bits[11], false);
  EXPECT_EQ(bits[12], false);
  EXPECT_EQ(bits[14], true);
  EXPECT_EQ(bits[17], true);
  EXPECT_EQ(bits[19], true);
  EXPECT_EQ(bits[21], true);
}

TEST(Bits, RightShift) {
  Bits bits = Bits::FromBinaryString("101010010001");
  bits >>= 10;
  EXPECT_EQ(bits[0], false);
  EXPECT_EQ(bits[1], true);
}

TEST(Bits, Xor) {
  Bits x = Bits::FromBinaryString("101010010001");
  Bits y = Bits::FromBinaryString("101010010001");
  x ^= y;
  EXPECT_EQ(x.ToString(), "000000000000");
}

TEST(Bits, Concat) {
  Bits x = Bits::FromBinaryString("101010010001");
  Bits y = Bits::FromBinaryString("101010010001");
  Bits z = x || y;
  EXPECT_EQ(z.ToString(), "101010010001101010010001");
}
