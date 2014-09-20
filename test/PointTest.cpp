#include "easyunit/test.h" 
#include "Point.h"
using namespace easyunit; 

TEST(Point,constructors_test) {
  Point p(1,2,3);
  ASSERT_EQUALS(1, p[X]);
  ASSERT_EQUALS(2, p[Y]);
  ASSERT_EQUALS(3, p[Z]);
}

TEST(Point,equality) {
  Point p1(1,2,3);
  Point p2(1,2,3);
  Point p3(1,2,1);
  ASSERT_EQUALS(p1, p2);
  ASSERT_EQUALS(p2, p1);
  ASSERT_TRUE(p1 != p3);
  ASSERT_TRUE(p3 != p1);
}
