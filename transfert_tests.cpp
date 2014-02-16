#include "transfert.h"
#include <gtest/gtest.h>

TEST(Transfert, rgbTolmsTorgb_for_black) {
  uchar r = 0;
  uchar g = 0;
  uchar b = 0;
  float l = ( rgbToL(r, g, b ));
  float m = ( rgbToM(r, g, b ));
  float s = ( rgbToS(r, g, b ));
  uchar _r = (uchar)lmsToR(l, m, s);
  uchar _g = (uchar)lmsToG(l, m, s);
  uchar _b = (uchar)lmsToB(l, m, s);
  ASSERT_EQ(r, _r);
  ASSERT_EQ(g ,_g);
  ASSERT_EQ(b ,_b);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
