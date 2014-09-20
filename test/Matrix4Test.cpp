#include "easyunit/test.h" 
#include "Matrix4.h"
#include "Point.h"
#include <cmath>
using namespace easyunit; 

TEST(Matrix4, constructor_test1) {
  Matrix4 m(1,2,3,4,
           2,4,6,8,
           3,6,9,12,
           4,8,12,16);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQUALS((i+1)*(j+1), m(i,j));
    }
  }
}

TEST(Matrix4, constructor_test2) {
  double elems[4][4] = {{1,2,3,4},
                        {2,4,6,8},
                        {3,6,9,12},
                        {4,8,12,16}};
  Matrix4 m(elems);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ASSERT_EQUALS((i+1)*(j+1), m(i,j));
    }
  }
}

TEST(Matrix4, matrix_mult) {
  Matrix4 m(1,2,3,4,
            5,6,7,8,
            9,8,7,6,
            5,4,3,2);
  ASSERT_EQUALS(m, m*identity());
  ASSERT_EQUALS(m, identity()*m);
}

TEST(Matrix4, point_mult) {
  Point p(1,2,3);
  ASSERT_EQUALS(p, identity()*p);
}

TEST(Matrix4, transpose) {
  Matrix4 m(1,2,3,4,
            5,6,7,8,
            9,8,7,6,
            5,4,3,2);
  Matrix4 m_transposed(1,5,9,5,
                       2,6,8,4,
                       3,7,7,3,
                       4,8,6,2);
  ASSERT_EQUALS(m_transposed, m.transpose());
  ASSERT_EQUALS(identity(), identity().transpose());
}

TEST(Matrix4, equals) {
  Matrix4 m1(1,2,3,4,
             5,6,7,8,
             9,8,7,6,
             5,4,3,2);
  Matrix4 m2(1,2,3,4,
             5,6,7,8,
             9,8,7,6,
             5,4,3,2);
  ASSERT_TRUE(m1 == m2);
  ASSERT_TRUE(m1 != m1.transpose());
}

TEST(Matrix4, identity) {
  Matrix4 i(1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1);
  ASSERT_EQUALS(i, identity());
}

TEST(Matrix4, rotatez) {
  Matrix4 expected(0,1,0,0,
                  -1,0,0,0,
                   0,0,0,0,
                   0,0,0,1);
  Matrix4 actual = rotateZ(-M_PI/2.0);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EQUALS_DELTA(expected(i,j), actual(i,j), 1e-10);
    }
  }
}

TEST(Matrix4, translate) {
  Matrix4 expected(1,0,0,10,
                   0,1,0,20,
                   0,0,1,30,
                   0,0,0,1);
  ASSERT_EQUALS(expected, translate(10,20,30));
}

TEST(Matrix4, scale) {
  Matrix4 expected(2,0,0,0,
                   0,4,0,0,
                   0,0,6,0,
                   0,0,0,1);
  ASSERT_EQUALS(expected, scale(2,4,6));
}
